import os

import numpy as np
import ROOT

import wafer

# Open the ROOT file
f1 = ROOT.TFile.Open("../root_files/Pedestals_with_BV.root")
t1 = f1.Get("PedestalTree")

# Initialize 2D arrays for processed data
Pedestals = [[0.0 for _ in range(222)] for _ in range(6)]
Pedestals_Error = [[0.0 for _ in range(222)] for _ in range(6)]
Total_Noise = [[0.0 for _ in range(222)] for _ in range(6)]
Bias_Voltage = [-130, -300, -400, -500, -600, -700]

ROOT.gStyle.SetOptStat(0)

print("Set up complete. Ready to create hexaplots.")

# Loop through entries
for file_number in range(6):
    t1.GetEntry(file_number)  # Load data for the current entry
    print(f"Retrieval of data from file number {file_number} complete")

    # Access data directly from branches
    pedestals = getattr(t1, "pedestals")
    pedestal_errors = getattr(t1, "pedestalErrors")
    noises = getattr(t1, "noises")

    # Check the sizes of the branches for debugging
    print(f"Size of pedestals: {len(pedestals)}")
    print(f"Size of pedestalErrors: {len(pedestal_errors)}")
    print(f"Size of noises: {len(noises)}")

    # Populate 2D arrays
    for m in range(6):
        for c in range(222):
            index = m * 222 + c
            if index < len(pedestals):
                Pedestals[m][c] = pedestals[index]
                Pedestals_Error[m][c] = pedestal_errors[index]
                Total_Noise[m][c] = noises[index]
            else:
                print(f"Index {index} out of range for entry {file_number}. Skipping.")
                continue

        # add a if statement here to keep only the noise of 22 noisiest channels
        sorted_total_noise = np.array(Total_Noise[m])
        top_c_indices = np.argsort(sorted_total_noise)[-44:]
        mask = np.zeros_like(sorted_total_noise)
        mask[top_c_indices] = sorted_total_noise[top_c_indices]
        Total_Noise[m] = mask

        print(
            f"Creating TH2 Object for Pedestals of file_number {file_number}, Module {m}"
        )
        hist = wafer.fill_wafer_hist(Pedestals[m])
        hist.SetTitle(f"Pedestals Map for Module {m}; BV = {Bias_Voltage[file_number]}")
        hist.SetMinimum(100)
        hist.SetMaximum(200)
        ROOT.gStyle.SetPaintTextFormat(".1f")
        canvas = ROOT.TCanvas(f"c_{file_number}_{m}", "Hexaplot", 800, 600)
        hist.Draw("COLZ TEXT")
        output_dir = "./Pedestals/"
        canvas.SaveAs(
            os.path.join(
                output_dir, f"Pedestals_BV_{Bias_Voltage[file_number]}_Module_{m}.png"
            )
        )

        print(f"Creating TH2 Object for Noise of file_number {file_number}, Module {m}")
        hist = wafer.fill_wafer_hist(Total_Noise[m])
        hist.SetTitle(
            f"Total_Noise Map for Module {m}; BV = {Bias_Voltage[file_number]}"
        )
        hist.SetMinimum(0)
        hist.SetMaximum(5)
        ROOT.gStyle.SetPaintTextFormat(".2f")
        canvas = ROOT.TCanvas(f"c_{file_number}_{m}_noise", "Hexaplot", 800, 600)
        hist.Draw("COLZ TEXT")
        output_dir = "./Total_Noise_with_mask20/"
        canvas.SaveAs(
            os.path.join(
                output_dir, f"Total_Noise_BV_{Bias_Voltage[file_number]}_Module_{m}.png"
            )
        )
