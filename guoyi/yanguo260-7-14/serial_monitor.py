#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
串口数据监控工具 - 用于 PID 调参
读取 COM12 无线串口数据，实时显示 Image_error、Turn_Out 等关键量

数据协议 (18字节/包):
  0xAA | ImgErr_H | ImgErr_L | TurnOut_H | TurnOut_L | gyro_z |
  R_Patch | L_Patch | R_Lost | L_Lost | White_MID | White_Nums |
  R_Local | L_Local | R_RingFlag | L_RingFlag | angle_ringR | 0xFF
"""

import serial
import struct
import time
import os
from datetime import datetime

# ==================== 配置 ====================
PORT = "COM12"
BAUD = 115200
# ==============================================

PACKET_LEN = 18

def parse_packet(data):
    """解析一个数据包，返回字典"""
    # 验证帧头帧尾
    if data[0] != 0xAA or data[17] != 0xFF:
        return None

    # 有符号 int16 恢复
    def to_int16(h, l):
        val = (h << 8) | l
        if val > 32767:
            val -= 65536
        return val

    image_error  = to_int16(data[1], data[2])
    turn_out     = to_int16(data[3], data[4])
    gyro_z       = data[5]       # uint8, 原始 avl_gyro_z 截断
    r_patch      = data[6]
    l_patch      = data[7]
    r_lost       = data[8]
    l_lost       = data[9]
    white_mid    = data[10]
    white_nums   = data[11]
    r_local      = data[12]
    l_local      = data[13]
    r_ring_flag  = data[14]
    l_ring_flag  = data[15]
    angle_ring   = data[16]      # uint8, 原始 angle_ringR 截断

    # 推断当前模式
    if l_ring_flag != 0 or r_ring_flag != 0:
        mode = "环岛"
    else:
        mode = "普通"

    return {
        "mode": mode,
        "image_error": image_error,
        "turn_out": turn_out,
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


def main():
    print(f"打开 {PORT} @ {BAUD} ...")
    ser = serial.Serial(PORT, BAUD, timeout=1)
    buf = bytearray()

    # 列标题
    header = (
        f"{'模式':<6} {'ImgErr':>7} {'TurnOut':>7} {'GyrZ':>4} "
        f"{'RPatch':>6} {'LPatch':>6} {'RLost':>5} {'LLost':>5} "
        f"{'WMid':>4} {'WNums':>5} {'RLoc':>5} {'LLoc':>5} "
        f"{'RFlag':>5} {'LFlag':>5} {'AngR':>4}"
    )
    sep = "-" * len(header.expandtabs())

    print("\n" + header)
    print(sep)

    count = 0
    while True:
        try:
            raw = ser.read(ser.in_waiting or 1)
            if not raw:
                continue
            buf.extend(raw)

            # 找帧头 0xAA
            while len(buf) >= PACKET_LEN:
                if buf[0] == 0xAA and buf[PACKET_LEN - 1] == 0xFF:
                    packet = parse_packet(buf[:PACKET_LEN])
                    buf = buf[PACKET_LEN:]
                    if packet is None:
                        continue

                    count += 1

                    # 每 15 行重新打印表头
                    if count % 15 == 1 and count > 1:
                        print(f"\n{header}\n{sep}")

                    line = (
                        f"{packet['mode']:<6} "
                        f"{packet['image_error']:>7} "
                        f"{packet['turn_out']:>7} "
                        f"{packet['gyro_z']:>4} "
                        f"{packet['r_patch']:>6} "
                        f"{packet['l_patch']:>6} "
                        f"{packet['r_lost']:>5} "
                        f"{packet['l_lost']:>5} "
                        f"{packet['white_mid']:>4} "
                        f"{packet['white_nums']:>5} "
                        f"{packet['r_local']:>5} "
                        f"{packet['l_local']:>5} "
                        f"{packet['r_ring_flag']:>5} "
                        f"{packet['l_ring_flag']:>5} "
                        f"{packet['angle_ring']:>4}"
                    )
                    print(line)
                else:
                    # 跳过一字节继续找
                    buf.pop(0)

        except KeyboardInterrupt:
            print(f"\n\n共收到 {count} 个数据包，关闭串口...")
            break
        except Exception as e:
            print(f"错误: {e}")
            time.sleep(0.5)

    ser.close()


if __name__ == "__main__":
    main()
