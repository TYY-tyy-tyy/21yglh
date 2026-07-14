#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
串口数据监控工具 - 用于 PID 调参
读取 COM12 无线串口数据，实时显示关键量

数据协议 (22字节/包):
  0xAA | ImgErr_H | ImgErr_L | TurnOut_H | TurnOut_L |
  EncL_H | EncL_L | EncR_H | EncR_L | gyro_z |
  R_Patch | L_Patch | R_Lost | L_Lost | White_MID | White_Nums |
  R_Local | L_Local | R_RingFlag | L_RingFlag | angle_ringR | 0xFF
"""

import serial
import time
import os
from datetime import datetime

# ==================== 配置 ====================
PORT = "COM12"
BAUD = 115200
# ==============================================

PACKET_LEN = 22
run_count = 0


def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")


def parse_packet(data):
    """解析一个数据包，返回字典"""
    if data[0] != 0xAA or data[21] != 0xFF:  # 0xFF 在索引 21
        return None

    def to_int16(h, l):
        val = (h << 8) | l
        if val > 32767:
            val -= 65536
        return val

    image_error  = to_int16(data[1], data[2])   # ImgErr
    turn_out     = to_int16(data[3], data[4])   # Turn_Out
    enc_left     = to_int16(data[5], data[6])   # 左轮编码器
    enc_right    = to_int16(data[7], data[8])   # 右轮编码器
    gyro_z       = data[9]                      # avl_gyro_z (uint8截断)
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

    # 差速
    enc_diff = enc_left - enc_right

    # 模式
    if l_ring_flag != 0 or r_ring_flag != 0:
        mode = "环岛"
    else:
        mode = "普通"

    return {
        "mode": mode,
        "image_error": image_error,
        "turn_out": turn_out,
        "enc_left": enc_left,
        "enc_right": enc_right,
        "enc_diff": enc_diff,
        "gyro_z": gyro_z,
        "r_patch": r_patch,
        "l_patch": l_patch,
        "r_lost": r_lost,
        "l_lost": l_lost,
        "white_mid": white_mid,
        "white_nums": white_nums,
        "r_local": r_local,
        "l_local": l_local,
        "r_ring_flag": r_ring_flag,
        "l_ring_flag": l_ring_flag,
        "angle_ring": angle_ring,
    }


def print_run_banner(run_num):
    ts = datetime.now().strftime("%H:%M:%S")
    print(f"\n\n  ╔══════════════════════════════════════════════════════════════════════╗")
    print(f"  ║  第 {run_num} 次跑车  |  开始时间: {ts}                                ║")
    print(f"  ╚══════════════════════════════════════════════════════════════════════╝")


def print_header():
    header = (
        f"{'模式':<6} {'ImgErr':>7} {'TurnOut':>7} {'EncL':>6} {'EncR':>6} {'Diff':>6} "
        f"{'GyrZ':>4} {'RPatch':>6} {'LPatch':>6} {'RLost':>5} {'LLost':>5} "
        f"{'WMid':>4} {'WNums':>5} {'RFlag':>5} {'LFlag':>5} {'AngR':>4}"
    )
    sep = "-" * len(header.expandtabs())
    print(f"\n{header}\n{sep}")


def main():
    global run_count

    clear_screen()
    print(f"  串口监控工具 — {PORT} @ {BAUD}")
    print(f"  等待小车数据... (Ctrl+C 退出)")
    print(f"  每次上电自动清屏，显示新跑次\n")

    ser = serial.Serial(PORT, BAUD, timeout=1)
    buf = bytearray()
    count = 0
    idle_counter = 0
    in_run = False
    run_count += 1

    print_run_banner(run_count)
    print_header()

    while True:
        try:
            raw = ser.read(ser.in_waiting or 1)
            if not raw:
                continue
            buf.extend(raw)

            while len(buf) >= PACKET_LEN:
                if buf[0] == 0xAA and buf[PACKET_LEN - 1] == 0xFF:
                    packet = parse_packet(buf[:PACKET_LEN])
                    buf = buf[PACKET_LEN:]

                    if packet is not None:
                        all_zero = (packet["image_error"] == 0
                                    and packet["turn_out"] == 0
                                    and packet["enc_left"] < 100
                                    and packet["enc_right"] < 100
                                    and packet["white_mid"] > 100
                                    and packet["r_ring_flag"] == 0
                                    and packet["l_ring_flag"] == 0)

                        if all_zero:
                            idle_counter += 1
                            if not in_run:
                                continue

                        # 从待机进入跑车 → 清屏 + 新 banner
                        if not in_run and not all_zero:
                            run_count += 1
                            clear_screen()
                            print_run_banner(run_count)
                            print_header()
                            in_run = True
                            count = 0

                        if in_run and all_zero:
                            idle_counter += 1
                        elif in_run and not all_zero:
                            idle_counter = 0

                        if idle_counter > 100 and in_run:
                            print(f"\n  >>> 第 {run_count} 次跑车结束，共 {count} 包 <<<")
                            print_header()
                            in_run = False
                            idle_counter = 0
                            continue

                        count += 1

                        if count % 15 == 1 and count > 1:
                            print_header()

                        line = (
                            f"{packet['mode']:<6} "
                            f"{packet['image_error']:>7} "
                            f"{packet['turn_out']:>7} "
                            f"{packet['enc_left']:>6} "
                            f"{packet['enc_right']:>6} "
                            f"{packet['enc_diff']:>6} "
                            f"{packet['gyro_z']:>4} "
                            f"{packet['r_patch']:>6} "
                            f"{packet['l_patch']:>6} "
                            f"{packet['r_lost']:>5} "
                            f"{packet['l_lost']:>5} "
                            f"{packet['white_mid']:>4} "
                            f"{packet['white_nums']:>5} "
                            f"{packet['r_ring_flag']:>5} "
                            f"{packet['l_ring_flag']:>5} "
                            f"{packet['angle_ring']:>4}"
                        )
                        print(line)
                else:
                    buf.pop(0)

        except KeyboardInterrupt:
            print(f"\n\n  共收到 {count} 个数据包，关闭串口...")
            break
        except Exception as e:
            print(f"  错误: {e}")
            time.sleep(0.5)

    ser.close()


if __name__ == "__main__":
    main()
