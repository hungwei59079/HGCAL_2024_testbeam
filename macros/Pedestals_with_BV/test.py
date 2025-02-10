import uproot
import numpy as np
import os
import glob
import matplotlib.pyplot as plt

# Define root directory
root_directory = "/data1/SepTB2024"
target_prefixes = ["NANO_172716", "NANO_172717", "NANO_172718", "NANO_172720"]

# Recursively find all ROOT files with the target prefixes
file_paths = []
for root, _, files in os.walk(root_directory):
    for filename in files:
        if filename.endswith(".root") and any(filename.startswith(prefix) for prefix in target_prefixes):
            file_paths.append(os.path.join(root, filename))

print("Found ROOT files:", file_paths)  # Debugging output

# Output directory for histograms
output_dir = os.path.expanduser("~/HGCAL/2024/root_files/histograms")
os.makedirs(output_dir, exist_ok=True)

# Histogram settings
bins = np.linspace(0, 300, 301)  # 300 bins from 0 to 300

# Process each file
for file_number, filepath in enumerate(file_paths):
    print(f"Processing file: {filepath}")
    
    with uproot.open(filepath) as f:
        if "Events" not in f:
            print(f"Warning: 'Events' tree not found in {filepath}")
            continue
        
        tree = f["Events"]

        # Load relevant branches
        adc = tree["HGCDigi_adc"].array(library="np")
        channel = tree["HGCDigi_channel"].array(library="np")
        fed_seq = tree["HGCDigi_fedReadoutSeq"].array(library="np")

        # First histogram (HGCDigi_channel == 74, fedReadoutSeq == 0)
        mask_1 = (channel == 74) & (fed_seq == 0)
        adc_values_1 = adc[mask_1]

        hist_1, _ = np.histogram(adc_values_1, bins=bins)
        print(f"Histogram h_ADC_1_{file_number} created with {np.sum(hist_1)} entries.")

        # Save histogram as PNG
        plt.figure(figsize=(8, 6))
        plt.hist(bins[:-1], bins, weights=hist_1, alpha=0.7, color="blue", label="Channel 74, FedReadoutSeq 0")
        plt.xlabel("ADC")
        plt.ylabel("Events")
        plt.legend()
        plt.title(f"ADC Histogram - File {file_number}")
        plt.savefig(os.path.join(output_dir, f"h_ADC_1_{file_number}.png"))
        plt.close()

        # Second histogram (HGCDigi_channel == 9, fedReadoutSeq == 5)
        mask_2 = (channel == 9) & (fed_seq == 5)
        adc_values_2 = adc[mask_2]

        hist_2, _ = np.histogram(adc_values_2, bins=bins)
        print(f"Histogram h_ADC_2_{file_number} created with {np.sum(hist_2)} entries.")

        # Save second histogram as PNG
        plt.figure(figsize=(8, 6))
        plt.hist(bins[:-1], bins, weights=hist_2, alpha=0.7, color="red", label="Channel 9, FedReadoutSeq 5")
        plt.xlabel("ADC")
        plt.ylabel("Events")
        plt.legend()
        plt.title(f"ADC Histogram - File {file_number}")
        plt.savefig(os.path.join(output_dir, f"h_ADC_2_{file_number}.png"))
        plt.close()

print(f"Histograms saved as PNGs in {output_dir}")

