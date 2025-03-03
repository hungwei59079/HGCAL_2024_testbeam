import os

import ROOT

import wafer

# Open the ROOT file
f1 = ROOT.TFile.Open("../root_files/TOT_analysis.root")
t1 = f1.Get("TOTTree")

Bias_Voltage = [-130, -300, -400, -500, -600, -700]

TOT_Events = [[0.0 for _ in range(222)] for _ in range(6)]
TOT_Mean = [[0.0 for _ in range(222)] for _ in range(6)]


ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPaintTextFormat(".2f")
print("Set up complete. Ready to create hexaplots.")

for file_number in range(6):
    t1.GetEntry(file_number)
    print(f"Retrieval of data from file number {file_number} complete")
    t_Events = getattr(t1, "TOT_events")
    t_Mean = getattr(t1, "TOT_mean")
    print(f"Size of t_Events: {len(t_Events)}")
    print(f"Size of t_Mean: {len(t_Mean)}")

    for m in range(6):
        for c in range(222):
            index = m * 222 + c
            if index < len(t_Events):
                TOT_Events[m][c] = t_Events[index]
                TOT_Mean[m][c] = t_Mean[index]
            else:
                print(f"Index {index} out of range for entry {file_number}. Skipping.")
                continue

        print(
            f"Creating TH2 Object for TOT_Events of file_number {file_number}, Module {m}"
        )
        hist = wafer.fill_wafer_hist(TOT_Events[m])
        hist.SetTitle(
            f"Total_Events Map for Module {m}; BV = {Bias_Voltage[file_number]}"
        )
        hist.SetMinimum(0)
        hist.SetMaximum(5)
        canvas = ROOT.TCanvas(f"c_{file_number}_{m}", "Hexaplot", 800, 600)
        hist.Draw("COLZ TEXT")
        output_dir = "./TOT_Events/"
        canvas.SaveAs(
            os.path.join(
                output_dir, f"TOT_Events_{Bias_Voltage[file_number]}_Module_{m}.png"
            )
        )

        print(
            f"Creating TH2 Object for TOT_Mean of file_number {file_number}, Module {m}"
        )
        hist = wafer.fill_wafer_hist(TOT_Mean[m])
        hist.SetTitle(f"TOT_Mean Map for Module {m}; BV = {Bias_Voltage[file_number]}")
        hist.SetMinimum(0)
        hist.SetMaximum(200)
        canvas = ROOT.TCanvas(f"c_{file_number}_{m}", "Hexaplot", 800, 600)
        hist.Draw("COLZ TEXT")
        output_dir = "./TOT_Mean/"
        canvas.SaveAs(
            os.path.join(
                output_dir, f"TOT_Mean_{Bias_Voltage[file_number]}_Module_{m}.png"
            )
        )
