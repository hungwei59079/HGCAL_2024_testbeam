import numpy as np
import uproot

# Open the ROOT file
root_directory = "/data1/SepTB2024"
target_prefixes = ["NANO_172716", "NANO_172717", "NANO_172718", "NANO_172720"]

# Recursively find all ROOT files with the target prefixes
file_paths = [
    "/data1/SepTB2024/Relay1727206089/d560f64b-e3dd-1ffa-98b4-85deaaaa1310/prompt/NANO_1727206092_1.root",
    "/data1/SepTB2024/Relay1727207396/6b641b3b-ab20-1d48-8546-a1a97c73708c/prompt/NANO_1727207399_1.root",
    "/data1/SepTB2024/Relay1727169610/7ba35975-cd2e-1163-858c-2e6e57ed22d0/prompt/NANO_1727169613_1.root",
    "/data1/SepTB2024/Relay1727170256/a3858cde-9be1-192e-923e-e3c934f4f040/prompt/NANO_1727170259_1.root",
    "/data1/SepTB2024/Relay1727173910/f5e77d4d-cf30-1f2d-9097-a70286ee6241/prompt/NANO_1727173913_1.root",
    "/data1/SepTB2024/Relay1727180386/e497cd2f-65f2-1f76-8336-5a0850b51605/prompt/NANO_1727180389_1.root",
]

bias = [-130, -300, -400, -500, -600, -700]

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
        common_mode = tree["HGCDigi_cm"].array(library="np")

        adc_flat = np.concatenate(adc)
        channel_flat = np.concatenate(channel)
        fed_seq_flat = np.concatenate(fed_seq)
        common_mode_flat = np.concatenate(common_mode)
        print(adc)
        print(adc_flat)
