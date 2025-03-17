import numpy as np

# Define the shape of the arrays
shape = (6, 6, 6)

# Load data from text files and reshape it
CM_Mean = np.loadtxt("CM_Mean.txt").reshape(shape)
CM_STDEV = np.loadtxt("CM_STDEV.txt").reshape(shape)

print("CM_Mean and CM_STDEV have been loaded successfully.")
