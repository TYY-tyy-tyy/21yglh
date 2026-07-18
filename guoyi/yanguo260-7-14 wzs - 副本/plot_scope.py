#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""generate oscilloscope-style waveform plots from CSV log"""

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import csv
import sys

def load_csv(path):
    rows = []
    with open(path, "r") as f:
        reader = csv.DictReader(f)
        for r in reader:
            rows.append(r)
    return rows

def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else "log/run_0715_232732.csv"
    data = load_csv(csv_path)
    n = len(data)

    # time axis (10ms per sample, starting from 0)
    t = np.arange(n) * 0.01

    # extract channels
    enc_l  = np.array([int(r["enc_left"]) for r in data])
    enc_r  = np.array([int(r["enc_right"]) for r in data])
    target = np.array([int(r["target_speed"]) for r in data])
    out_l  = np.array([int(r["speed_out_l"]) for r in data])
    out_r  = np.array([int(r["speed_out_r"]) for r in data])
    img_err = np.array([int(r["image_error"]) for r in data])
    turn_out = np.array([int(r["turn_out"]) for r in data])
    enc_avg = (enc_l + enc_r) / 2.0
    mode_names = {0:"curve", 1:"straight", 2:"ring", 3:"big_curve"}
    modes = np.array([int(r["speed_mode"]) for r in data])

    # dark scope theme
    plt.rcParams.update({
        "figure.facecolor": "#1a1a2e",
        "axes.facecolor": "#16213e",
        "axes.edgecolor": "#4a9eff",
        "axes.labelcolor": "#aaccff",
        "text.color": "#aaccff",
        "xtick.color": "#8899bb",
        "ytick.color": "#8899bb",
        "grid.color": "#2a3a5e",
        "grid.alpha": 0.6,
        "legend.facecolor": "#1a1a2e",
        "legend.edgecolor": "#4a9eff",
        "legend.labelcolor": "#aaccff",
    })

    fig, axes = plt.subplots(3, 1, figsize=(22, 12), sharex=True)
    ax1, ax2, ax3 = axes

    # ---- Channel 1: Encoder vs Target ----
    ax1.plot(t, target, color="#ff6666", linewidth=1.8, alpha=0.9, label="target_speed")
    ax1.plot(t, enc_l,  color="#44ff44", linewidth=0.8, alpha=0.85, label="enc_L")
    ax1.plot(t, enc_r,  color="#44aaff", linewidth=0.8, alpha=0.85, label="enc_R")
    ax1.set_ylabel("Encoder / Target", fontsize=11)
    ax1.legend(loc="upper right", ncol=3, fontsize=9)
    ax1.grid(True, linestyle="--", linewidth=0.5)
    ax1.set_ylim(bottom=0)

    # mark mode changes with background color
    for i in range(1, n):
        if modes[i] != modes[i-1]:
            ax1.axvline(x=t[i], color="#ffaa00", linewidth=0.5, alpha=0.3, linestyle="--")

    # ---- Channel 2: PWM Speed Output ----
    ax2.fill_between(t, 0, out_l, color="#44ff44", alpha=0.2)
    ax2.fill_between(t, 0, out_r, color="#44aaff", alpha=0.2)
    ax2.plot(t, out_l, color="#44ff44", linewidth=0.8, label="speed_out_L")
    ax2.plot(t, out_r, color="#44aaff", linewidth=0.8, label="speed_out_R")
    ax2.axhline(y=7000, color="#ff4444", linewidth=0.6, linestyle=":", alpha=0.5, label="limit +/-7000")
    ax2.axhline(y=-7000, color="#ff4444", linewidth=0.6, linestyle=":", alpha=0.5)
    ax2.set_ylabel("PWM Speed Out", fontsize=11)
    ax2.legend(loc="upper right", ncol=3, fontsize=9)
    ax2.grid(True, linestyle="--", linewidth=0.5)

    # ---- Channel 3: Image Error + Turn Out ----
    ax3.plot(t, img_err,  color="#ffaa00", linewidth=1.0, alpha=0.9, label="image_error")
    ax3.plot(t, turn_out, color="#ff66cc", linewidth=1.0, alpha=0.9, label="turn_out")
    ax3.axhline(y=0, color="#666666", linewidth=0.5, linestyle="-")
    ax3.set_ylabel("Error / Turn", fontsize=11)
    ax3.set_xlabel("Time (s)", fontsize=11)
    ax3.legend(loc="upper right", ncol=2, fontsize=9)
    ax3.grid(True, linestyle="--", linewidth=0.5)

    # title
    fig.suptitle(f"Oscilloscope - {csv_path}  (N={n}, T={t[-1]:.1f}s)",
                 fontsize=14, color="#4acfff", fontweight="bold")

    # tight layout
    plt.tight_layout()
    plt.subplots_adjust(top=0.94)

    # save
    out_name = csv_path.replace(".csv", "_scope.png").replace("log/", "log/")
    fig.savefig(out_name, dpi=150, facecolor="#1a1a2e", edgecolor="none")
    print(f"Saved: {out_name}")
    plt.close()


if __name__ == "__main__":
    main()
