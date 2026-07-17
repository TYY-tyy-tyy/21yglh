#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
serial monitor for PID tuning
auto-detect runs + export CSV

Protocol (29 bytes/packet):
  0xAA | ImgErr_H | ImgErr_L | TurnOut_H | TurnOut_L |
  EncL_H | EncL_L | EncR_H | EncR_L | gyro_z |
  R_Patch | L_Patch | R_Lost | L_Lost | White_MID | White_Nums |
  R_Local | L_Local | R_RingFlag | L_RingFlag | angle_ringR | speed_mode |
  TargetSpeed_H | TargetSpeed_L | SpeedL_H | SpeedL_L | SpeedR_H | SpeedR_L | 0xFF
"""

import serial
import time
import os
from datetime import datetime

# ==================== config ====================
PORT = "COM12"
BAUD = 115200
IDLE_TIMEOUT = 2.0
LOG_DIR = "log"
# ================================================

PACKET_LEN = 29
run_count = 0

SPEED_MODE_NAMES = {0: "curve", 1: "straight", 2: "ring", 3: "big_curve"}

PARKING_ENC_THRESHOLD = 3

CSV_HEADER = (
    "timestamp,speed_mode,image_error,turn_out,enc_left,enc_right,enc_diff,"
    "gyro_z,r_patch,l_patch,r_lost,l_lost,white_mid,white_nums,"
    "r_local,l_local,r_ring_flag,l_ring_flag,angle_ring,"
    "target_speed,speed_out_l,speed_out_r\n"
)


def ensure_log_dir():
    os.makedirs(LOG_DIR, exist_ok=True)


def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")


def parse_packet(data):
    if data[0] != 0xAA or data[28] != 0xFF:
        return None

    def to_int16(h, l):
        val = (h << 8) | l
        if val > 32767:
            val -= 65536
        return val

    image_error  = to_int16(data[1], data[2])
    turn_out     = to_int16(data[3], data[4])
    enc_left     = to_int16(data[5], data[6])
    enc_right    = to_int16(data[7], data[8])
    gyro_z       = data[9]
    r_patch      = data[10]
    l_patch      = data[11]
    r_lost       = data[12]
    l_lost       = data[13]
    white_mid    = data[14]
    white_nums   = data[15]
    r_local      = data[16]
    l_local      = data[17]
    r_ring_flag  = data[18]
    l_ring_flag  = data[19]
    angle_ring   = data[20]
    speed_mode   = data[21]
    target_speed = to_int16(data[22], data[23])
    speed_out_l  = to_int16(data[24], data[25])
    speed_out_r  = to_int16(data[26], data[27])
    enc_diff     = enc_left - enc_right

    mode = SPEED_MODE_NAMES.get(speed_mode, "???")

    return {
        "mode": mode, "speed_mode": speed_mode,
        "image_error": image_error, "turn_out": turn_out,
        "enc_left": enc_left, "enc_right": enc_right, "enc_diff": enc_diff,
        "gyro_z": gyro_z, "r_patch": r_patch, "l_patch": l_patch,
        "r_lost": r_lost, "l_lost": l_lost, "white_mid": white_mid,
        "white_nums": white_nums, "r_local": r_local, "l_local": l_local,
        "r_ring_flag": r_ring_flag, "l_ring_flag": l_ring_flag,
        "angle_ring": angle_ring, "target_speed": target_speed,
        "speed_out_l": speed_out_l, "speed_out_r": speed_out_r,
    }


# ==================== info boxes ====================

def print_info_boxes(p):
    W = 52
    sep  = "+" + "-" * W + "+"

    # Box 1: target speed / encoder / pwm output
    print(sep)
    print("|  [Speed & PWM Output]")
    print("|  Target: {:<6}  ENC_L: {:<6} ENC_R: {:<6}  OUT_L: {:<7} OUT_R: {:<7}".format(
        p['target_speed'], p['enc_left'], p['enc_right'],
        p['speed_out_l'], p['speed_out_r']))
    print(sep)

    # Box 2: image error / turn out
    print("|  [Error & Turn]")
    print("|  Image_error: {:<6}      Turn_Out: {:<6}".format(
        p['image_error'], p['turn_out']))
    print(sep)

    # mode info
    print("|  Mode: {:<8}  GyroZ: {:<4}".format(p['mode'], p['gyro_z']))
    print(sep)
    print()


def print_header():
    hdr = (
        "{:<8} {:>7} {:>7} {:>6} {:>6} {:>6} "
        "{:>4} {:>6} {:>6} {:>5} {:>5} "
        "{:>4} {:>5} {:>5} {:>5} {:>4} "
        "{:>7} {:>7}"
    ).format(
        "Mode", "ImgErr", "TurnOut", "EncL", "EncR", "Diff",
        "GyrZ", "RPatch", "LPatch", "RLost", "LLost",
        "WMid", "WNums", "RFlg", "LFlg", "AngR",
        "SpeedL", "SpeedR"
    )
    print(hdr)
    print("-" * 120)


def format_line(p):
    return (
        "{:<8} {:>7} {:>7} {:>6} {:>6} {:>6} "
        "{:>4} {:>6} {:>6} {:>5} {:>5} "
        "{:>4} {:>5} {:>5} {:>5} {:>4} "
        "{:>7} {:>7}"
    ).format(
        p['mode'], p['image_error'], p['turn_out'],
        p['enc_left'], p['enc_right'], p['enc_diff'],
        p['gyro_z'], p['r_patch'], p['l_patch'],
        p['r_lost'], p['l_lost'], p['white_mid'],
        p['white_nums'], p['r_ring_flag'], p['l_ring_flag'],
        p['angle_ring'], p['speed_out_l'], p['speed_out_r']
    )


def format_csv(p):
    return (
        f"{datetime.now().strftime('%H:%M:%S.%f')[:-3]},"
        f"{p['speed_mode']},"
        f"{p['image_error']},{p['turn_out']},"
        f"{p['enc_left']},{p['enc_right']},{p['enc_diff']},"
        f"{p['gyro_z']},"
        f"{p['r_patch']},{p['l_patch']},"
        f"{p['r_lost']},{p['l_lost']},"
        f"{p['white_mid']},{p['white_nums']},"
        f"{p['r_local']},{p['l_local']},"
        f"{p['r_ring_flag']},{p['l_ring_flag']},"
        f"{p['angle_ring']},"
        f"{p['target_speed']},"
        f"{p['speed_out_l']},{p['speed_out_r']}\n"
    )


def filter_parking_rows(csv_path):
    try:
        with open(csv_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
    except Exception:
        return 0

    header = lines[0] if lines else ""
    kept = [header]
    removed = 0

    for line in lines[1:]:
        parts = line.strip().split(",")
        if len(parts) < 6:
            removed += 1
            continue
        try:
            enc_left  = int(parts[4])
            enc_right = int(parts[5])
        except ValueError:
            removed += 1
            continue

        if abs(enc_left) <= PARKING_ENC_THRESHOLD and abs(enc_right) <= PARKING_ENC_THRESHOLD:
            removed += 1
        else:
            kept.append(line)

    if removed > 0:
        with open(csv_path, "w", encoding="utf-8") as f:
            f.writelines(kept)

    return removed


def start_new_run():
    global run_count
    run_count += 1
    clear_screen()
    ts = datetime.now().strftime("%H:%M:%S")
    fname = datetime.now().strftime("run_%m%d_%H%M%S.csv")
    csv_path = os.path.join(LOG_DIR, fname)
    print(f"\n  ===== Run #{run_count} | Start: {ts} =====")
    print(f"  File: {csv_path}")
    return csv_path


def main():
    global run_count

    ensure_log_dir()
    clear_screen()
    print(f"  Serial Monitor -- {PORT} @ {BAUD}")
    print(f"  Auto-detect runs + Export CSV to {LOG_DIR}/")
    print(f"  {IDLE_TIMEOUT}s idle = end run, new data = new run + clear")
    print(f"  Ctrl+C to exit\n")

    ser = serial.Serial(PORT, BAUD, timeout=0.5)
    buf = bytearray()
    count = 0
    last_pkt_time = 0
    in_run = False
    csv_fh = None
    csv_path = None

    while True:
        try:
            raw = ser.read(1024)
        except KeyboardInterrupt:
            break
        except Exception:
            time.sleep(0.1)
            continue

        now = time.time()

        if raw:
            buf.extend(raw)

        while len(buf) >= PACKET_LEN:
            if buf[0] == 0xAA and buf[PACKET_LEN - 1] == 0xFF:
                pkt = parse_packet(buf[:PACKET_LEN])
                buf = buf[PACKET_LEN:]
                if pkt is None:
                    continue

                gap = now - last_pkt_time
                if in_run and gap > IDLE_TIMEOUT:
                    if csv_fh:
                        csv_fh.close()
                        csv_fh = None
                    removed = filter_parking_rows(csv_path)
                    print(f"\n  >>> Run #{run_count} end (pkt={count}, removed={removed}) <<<")
                    in_run = False

                if not in_run:
                    csv_path = start_new_run()
                    csv_fh = open(csv_path, "w", encoding="utf-8")
                    csv_fh.write(CSV_HEADER)
                    in_run = True
                    count = 0

                last_pkt_time = now
                count += 1

                if csv_fh:
                    csv_fh.write(format_csv(pkt))

                if count % 15 == 1:
                    print_info_boxes(pkt)
                    print_header()

                print(format_line(pkt))
            else:
                buf.pop(0)

        if in_run and now - last_pkt_time > IDLE_TIMEOUT:
            if csv_fh:
                csv_fh.close()
                csv_fh = None
            removed = filter_parking_rows(csv_path)
            print(f"\n  >>> Run #{run_count} end (pkt={count}, removed={removed}) <<<")
            in_run = False

        time.sleep(0.001)

    if csv_fh:
        csv_fh.close()
    print(f"\n  Exit.")
    ser.close()


if __name__ == "__main__":
    main()
