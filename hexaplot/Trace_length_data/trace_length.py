import os
import sys

import matplotlib.pyplot as plt
import pandas as pd
import ROOT

parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, parent_dir)

import wafer

# Define column names
columns = [
    "Dens",
    "Wtype",
    "ROC",
    "HalfROC",
    "Seq",
    "ROCpin",
    "SiCell",
    "TrLink",
    "TrCell",
    "iu",
    "iv",
    "trace",
    "t",
]

# Read the dataset into a DataFrame
df = pd.read_csv(
    "WaferCellMapTrg.txt", delim_whitespace=True, names=columns, skiprows=1
)

# Now you can access columns easily, e.g., df['trace'], df['SiCell']

# Save the DataFrame to a CSV file for future use
df.to_csv("dataset.csv", index=False)

# Save the DataFrame to a dictionary of lists if you prefer that structure
data_dict = df.to_dict(orient="list")

# trace length map
trace_values = data_dict["trace"][:222]
# print(trace_values)
# hist = wafer.fill_wafer_hist(trace_values)
# wafer.draw_and_save(hist, "trace_length", minimum=0, maximum=80)
#
f1 = ROOT.TFile.Open("../../root_files/Pedestals_with_BV.root")
t1 = f1.Get("PedestalTree")

Total_Noise = [[0.0 for _ in range(222)] for _ in range(6)]
Bias_Voltage = [-130, -300, -400, -500, -600, -700]

for file_number in range(6):
    t1.GetEntry(file_number)
    noises = getattr(t1, "noises")
    for m in range(6):
        for c in range(222):
            index = m * 222 + c
            Total_Noise[m][c] = noises[index]
        plt.figure(figsize=(8, 6))
        plt.scatter(trace_values, Total_Noise[m])
        plt.xlabel("trace length (cm)")
        plt.ylabel("Noise (ADC)")
        title = f"Bias_{Bias_Voltage[file_number]}_module_{m}_trace_correlation"
        plt.title(title)
        plt.savefig(title)
        plt.close()
