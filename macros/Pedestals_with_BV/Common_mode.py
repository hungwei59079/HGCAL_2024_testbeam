import os

import matplotlib.pyplot as plt
import numpy as np
import uproot

file_paths = [
    "/data1/SepTB2024/Relay1727206089/d560f64b-e3dd-1ffa-98b4-85deaaaa1310/prompt/NANO_1727206092_1.root",
    "/data1/SepTB2024/Relay1727207396/6b641b3b-ab20-1d48-8546-a1a97c73708c/prompt/NANO_1727207399_1.root",
    "/data1/SepTB2024/Relay1727169610/7ba35975-cd2e-1163-858c-2e6e57ed22d0/prompt/NANO_1727169613_1.root",
    "/data1/SepTB2024/Relay1727170256/a3858cde-9be1-192e-923e-e3c934f4f040/prompt/NANO_1727170259_1.root",
    "/data1/SepTB2024/Relay1727173910/f5e77d4d-cf30-1f2d-9097-a70286ee6241/prompt/NANO_1727173913_1.root",
    "/data1/SepTB2024/Relay1727180386/e497cd2f-65f2-1f76-8336-5a0850b51605/prompt/NANO_1727180389_1.root",
]

bias = [-130, -300, -400, -500, -600, -700]
CM_Mean = np.empty((6, 6, 6))
CM_STDEV = np.empty((6, 6, 6))
# Process each file
for file_number, filepath in enumerate(file_paths):
    print(f"\nProcessing file {file_number + 1}/{len(file_paths)}: {filepath}")

    with uproot.open(filepath) as f:
        if "Events" not in f:
            print(f"Warning: 'Events' tree not found in {filepath}")
            continue

        tree = f["Events"]
        histogram_data = [[] for _ in range(36)]
        np_histogram_data = [[] for _ in range(36)]

        print("Loading common_mode data...")
        common_mode = tree["HGCDigi_cm"].array(library="np") / 2
        print(f"Loaded {len(common_mode)} events.")

        # Loop through events
        for event_number in range(len(common_mode)):
            if event_number % 1000 == 0:
                print(f"Processing event {event_number}/{len(common_mode)}...")

            for i in range(36):
                index = 37 * i  # Get the correct index
                if index < len(
                    common_mode[event_number]
                ):  # Ensure index is within bounds
                    histogram_data[i].append(common_mode[event_number][index])
                else:
                    print(
                        f"Warning: Index {index} out of range in event {event_number}"
                    )

        print("Finished processing events. Now saving histograms...")

        for m in range(6):
            for half_roc in range(6):
                i = m * 6 + half_roc
                np_histogram_data[i] = np.array(histogram_data[i])
                CM_Mean[file_number][m][half_roc] = np.mean(np_histogram_data[i])
                CM_STDEV[file_number][m][half_roc] = np.std(np_histogram_data[i])

                # print(f"Saving histogram {i + 1}/36 for file {file_number}...")
                # plt.figure()
                # plt.hist(
                #     histogram_data[i],
                #     bins=1024,
                #     range=(0, 1023),
                #     alpha=0.7,
                #     color="blue",
                # )
                # plt.title(f"Bias: {bias[file_number]}; Module {m}; Half_roc {half_roc}")
                # plt.xlabel("ADC")
                # plt.ylabel("Events")
                # plt.savefig(
                #     f"CM_Bias_{bias[file_number]}V_mod_{m}_halfroc_{half_roc}.png"
                # )
                # plt.close()

    print(f"Finished processing file {file_number + 1}/{len(file_paths)}.\n")

os.chdir("../../hexaplot/")
np.savetxt("CM_Mean.txt", CM_Mean.flatten(), fmt="%.6f")
np.savetxt("CM_STDEV.txt", CM_STDEV.flatten(), fmt="%.6f")
