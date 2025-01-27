import os
import sys

import ROOT

sys.path.append("../../hexaplot/")
import wafer

# Open the ROOT file
f1 = ROOT.TFile.Open(
    "/data1/SepTB2024/Relay1726920851/NANO_1726920854_1.root"
)  # Fix this with complete path
t1 = f1.Get("Events")


ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPaintTextFormat(".2f")

for i in range(0, 10):
    t1.GetEntry(i)
    ADC = getattr(t1, "HGCDigi_adc")
    channel = getattr(t1, "HGCDigi_channel")
    print(f"Event {i}; Channels: {channel}; ADC: {ADC}")
