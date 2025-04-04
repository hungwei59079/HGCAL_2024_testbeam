import uproot

# Open the ROOT file
file_path = "/data1/SepTB2024/Relay1726920851/NANO_1726920854_1.root"
with uproot.open(file_path) as file:
    # Access the "Events" tree
    t1 = file["Events"]
    
    # Access branches directly
    adc_branch = t1["HGCDigi_adc"].array()
    channel_branch = t1["HGCDigi_channel"].array()
    module_branch = t1["HGCDigi_fedReadoutSeq"].array()

    # Loop over events (limited to the first 10)
    for i in range(200):
        adc = adc_branch[i]  # ADC values for event i
        channel = channel_branch[i]  # Channel values for event i
        module_number = module_branch[i]
        print(f"Event {i}; module numbers: {module_number}; Channels: {channel}; ADC: {adc}")
        print(f"Type of channel: {type(channel)}; Type of adc: {type(adc)}")
