import os

import matplotlib.pyplot as plt
import numpy as np
import ROOT

import wafer

# Define the shape of the arrays
shape = (6, 6, 6)

ROOT.gStyle.SetOptStat(0)
# Load data from text files and reshape it
CM_Mean = np.loadtxt("CM_Mean.txt").reshape(shape)
CM_STDEV = np.loadtxt("CM_STDEV.txt").reshape(shape)

Bias_Voltage = [130, 300, 400, 500, 600, 700]
values_to_draw = np.empty(222)

# hexaplot version

# for file_number in range(6):
#     for module in range(6):
#         for half_roc in range(6):
#             for channel in range(37):
#                 values_to_draw[half_roc * 37 + channel] = CM_STDEV[file_number][module][
#                     half_roc
#                 ]
#         hist = wafer.fill_wafer_hist(values_to_draw)
#         title = f"CM_noise_map_Bias_{Bias_Voltage[file_number]}_Module_{module}"
#         wafer.draw_and_save(
#             hist,
#             title,
#             PaintFormat=".2f",
#             output_dir="./CM_Noise/",
#             minimum=0,
#             maximum=5,
#         )
#
#         for half_roc in range(6):
#             for channel in range(37):
#                 values_to_draw[half_roc * 37 + channel] = CM_Mean[file_number][module][
#                     half_roc
#                 ]
#         hist = wafer.fill_wafer_hist(values_to_draw)
#         title = f"CM_value_map_Bias_{Bias_Voltage[file_number]}_Module_{module}"
#         wafer.draw_and_save(
#             hist,
#             title,
#             PaintFormat=".1f",
#             output_dir="./CM_values/",
#             minimum=100,
#             maximum=200,
#         )

# Scatter plot version
os.chdir("./CM_Noise_graphs/")
for module in range(6):
    plt.figure(figsize=(8, 6))
    for half_roc in range(6):
        plt.scatter(
            Bias_Voltage, CM_STDEV[:, module, half_roc], label=f"Half ROC {half_roc}"
        )
        plt.plot(Bias_Voltage, CM_STDEV[:, module, half_roc], linestyle="-", marker="o")

    plt.xlabel("Bias Voltage (V)")
    plt.ylabel("Noise (ADC)")
    plt.title(f"CM Noise for Module {module}")
    plt.ylim(0.5, 2.5)
    plt.legend()
    plt.grid(True)
    plt.savefig(f"CM_Noise_Module_{module}.png")
    plt.close()
