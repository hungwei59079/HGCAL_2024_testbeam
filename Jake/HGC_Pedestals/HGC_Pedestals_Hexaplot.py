import os
import sys

script_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, script_dir)

import numpy as np

import ROOT
import wafer

if len(sys.argv) != 2:
    print(f"Usage: python3 {sys.argv[0]} <some_of_all_STDev.txt>")
    sys.exit(1)

HGC_STDev = np.loadtxt(sys.argv[1], delimiter=" ", dtype=float)

for i in range(6):
    hist = wafer.fill_wafer_hist(HGC_STDev[i])
    hist.SetTitle(f"STDev Map for Module {i}")
    hist.SetMinimum(0)
    hist.SetMaximum(0.4)
    canvas = ROOT.TCanvas(f"m_{i}", "Hexaplot", 800, 600)
    hist.Draw("COLZ TEXT")
    output_dir = "."
    canvas.SaveAs(
        os.path.join(
            output_dir, f"STDev_Map_for_Module_{i}.png"
        )
    )
