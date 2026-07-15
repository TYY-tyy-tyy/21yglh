#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
串口数据监控工具 - 用于 PID 调参
无线串口，自动检测跑次 + 导出 CSV 供分析

数据协议 (23字节/包):
  0xAA | ImgErr_H | ImgErr_L | TurnOut_H | TurnOut_L |
  EncL_H | EncL_L | EncR_H | EncR_L | gyro_z |
  R_Patch | L_Patch | R_Lost | L_Lost | White_MID | White_Nums |
  R_Local | L_Local | R_RingFlag | L_RingFlag | angle_ringR | speed_mode | 0xFF
"""

import serial
import time
import os
from datetime import datetime

# ==================== 配置 ====================
PORT = "COM12"
BAUD = 115200
IDLE_TIMEOUT = 2.0       # 连续无数据超过此秒 → 判定本次跑车结束
LOG_DIR = "log"          # CSV 保存目录
# ==============================================

PACKET_LEN = 23
run_count = 0

SPEED_MODE_NAMES = {0: "弯道", 1: "直道", 2: "环岛", 3: "大弯"}

# 停车段过滤: |enc| <= 此值视为停车
PARKING_ENC_THRESHOLD = 3

CSV_HEADER = (
    "timestamp,speed_mode,image_error,turn_out,enc_left,enc_right,enc_diff,"
    "gyro_z,r_patch,l_patch,r_lost,l_lost,white_mid,white_nums,"
    "r_local,l_local,r_ring_flag,l_ring_flag,angle_ring\n"
)


def ensure_log_dir():
    os.makedirs(LOG_DIR, exist_ok=True)


def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")


def parse_packet(data):
    if data[0] != 0xAA or data[22] != 0xFF:
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
    speed_mode   = data[21]   # 0=弯道 1=直道 2=环岛 3=大弯
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
        "angle_ring": angle_ring,
    }


def print_header():
    hdr = (
        f"{'模式':<6} {'ImgErr':>7} {'TurnOut':>7} {'EncL':>6} {'EncR':>6} {'Diff':>6} "
        f"{'GyrZ':>4} {'RPatch':>6} {'LPatch':>6} {'RLost':>5} {'LLost':>5} "
        f"{'WMid':>4} {'WNums':>5} {'RFlag':>5} {'LFlag':>5} {'AngR':>4}"
    )
    print(f"\n{hdr}\n{'-' * 112}")


def format_line(p):
    return (
        f"{p['mode']:<6} {p['image_error']:>7} {p['turn_out']:>7} "
        f"{p['enc_left']:>6} {p['enc_right']:>6} {p['enc_diff']:>6} "
        f"{p['gyro_z']:>4} {p['r_patch']:>6} {p['l_patch']:>6} "
        f"{p['r_lost']:>5} {p['l_lost']:>5} {p['white_mid']:>4} "
        f"{p['white_nums']:>5} {p['r_ring_flag']:>5} {p['l_ring_flag']:>5} "
        f"{p['angle_ring']:>4}"
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
        f"{p['angle_ring']}\n"
    )


def filter_parking_rows(csv_path):
    """移除编码值为0的停车行，原地覆盖"""
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
            enc_left  = int(parts[4])   # CSV 第5列
            enc_right = int(parts[5])   # CSV 第6列
        except ValueError:
            removed += 1
            continue

        # 两个编码器都在停车阈值内 → 过滤掉
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
    print(f"\n  ╔══════════════════════════════════════════════════════════╗")
    print(f"  ║  第 {run_count} 次跑车  |  开始: {ts}  ║")
    print(f"  ║  保存: {csv_path}  ║")
    print(f"  ╚══════════════════════════════════════════════════════════╝")
    print_header()
    return csv_path


def main():
    global run_count

    ensure_log_dir()
    clear_screen()
    print(f"  串口监控工具 — {PORT} @ {BAUD} (无线串口)")
    print(f"  自动检测跑次 + 导出 CSV 到 {LOG_DIR}/")
    print(f"  {IDLE_TIMEOUT}s 无数据 = 结束, 再来数据 = 新跑次 + 清屏")
    print(f"  模式: 直道 / 弯道 / 环岛 / 大弯 (由 MCU side 判断)")
    print(f"  Ctrl+C 退出\n")

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
                    print(f"\n  >>> 第 {run_count} 次跑车结束 (共 {count} 包, 过滤 {removed} 行停车数据) <<<")
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

                if count % 15 == 1 and count > 1:
                    print_header()

                print(format_line(pkt))
            else:
                buf.pop(0)

        if in_run and now - last_pkt_time > IDLE_TIMEOUT:
            if csv_fh:
                csv_fh.close()
                csv_fh = None
            removed = filter_parking_rows(csv_path)
            print(f"\n  >>> 第 {run_count} 次跑车结束 (共 {count} 包, 过滤 {removed} 行停车数据) <<<")
            in_run = False

        time.sleep(0.001)

    if csv_fh:
        csv_fh.close()
    print(f"\n  退出。")
    ser.close()


if __name__ == "__main__":
    main()
