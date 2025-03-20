import os

import numpy as np
import ROOT

import wafer

# Define the shape of the arrays
shape = (6, 6, 6)

ROOT.gStyle.SetOptStat(0)
# Load data from text files and reshape it
CM_Mean = np.loadtxt("CM_Mean.txt").reshape(shape)
CM_STDEV = np.loadtxt("CM_STDEV.txt").reshape(shape)

Bias_Voltage = [-130, -300, -400, -500, -600, -700]
values_to_draw = np.empty(222)

for file_number in range(6):
    for module in range(6):
        for half_roc in range(6):
            for channel in range(37):
                values_to_draw[half_roc * 37 + channel] = CM_STDEV[file_number][module][
                    half_roc
                ]
        hist = wafer.fill_wafer_hist(values_to_draw)
        title = f"CM_noise_map_Bias_{Bias_Voltage[file_number]}_Module_{module}"
        wafer.draw_and_save(
            hist,
            title,
            PaintFormat=".2f",
            output_dir="./CM_Noise/",
            minimum=0,
            maximum=3,
        )

        for half_roc in range(6):
            for channel in range(37):
                values_to_draw[half_roc * 37 + channel] = CM_Mean[file_number][module][
                    half_roc
                ]
        hist = wafer.fill_wafer_hist(values_to_draw)
        title = f"CM_value_map_Bias_{Bias_Voltage[file_number]}_Module_{module}"
        wafer.draw_and_save(
            hist,
            title,
            PaintFormat=".1f",
            output_dir="./CM_values/",
            minimum=100,
            maximum=200,
        )
