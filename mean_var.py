import numpy as np

# Lists for names, n, and m
dirs = ["results_determinista/results_determinista-", "results_rand/results_rand-"]
thresholds = ["0.75", "0.8", "0.85"]
n = ["100", "200"]
m = ["300", "600", "800"]

# Loop through each combination of n and m
for name in dirs:
    for th in thresholds:
        for i in n:
            for j in m:
                # Construct the filename
                filename = name + i + "-" + j + "-" + th + ".txt"
    
                # Read the numbers from the file
                with open(filename, 'r') as file:
                    numbers = [float(line.strip().replace("%", "")) for line in file]
    
                # Calculate the mean using numpy
                mean_value = np.mean(numbers)
    
                # Calculate the variance using numpy
                variance_value = np.var(numbers)
    
                # Print the result (mean and variance)
                print(f"{filename} - Mean: {mean_value}, Variance: {variance_value}")
