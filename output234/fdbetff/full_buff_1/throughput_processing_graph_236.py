import os

# Get the current working directory
folder = os.getcwd()

# Iterate over each file in the current directory
for filename in os.listdir(folder):
    # Check if the filename does not start with "throughput_"
    if not filename.startswith("throughput_"):
        # Construct the full path to the file
        filepath = os.path.join(folder, filename)
        
        # Check if the file is a regular file (not a directory)
        if os.path.isfile(filepath):
            # Execute the grep command to search for lines containing "Throughput"
            # and write them to a new file with "throughput_" prepended to the filename
            os.system('grep -i "Throughput" {} > {}'.format(filepath, os.path.join(folder, 'throughput_' + filename)))

# Now we have files with only throughput values, let's process them and calculate cumulative frequency
for filename in os.listdir(folder):
    if filename.startswith("throughput_"):
        input_filepath = os.path.join(folder, filename)
        output_filepath = os.path.join(folder, 'cumulative_frequency_' + filename[11:])
        
        with open(input_filepath, 'r') as input_file, open(output_filepath, 'w') as output_file:
            throughput_values = []
            for line in input_file:
                throughput = float(line.split(":")[1].strip().split()[0][:-3])  # Extract throughput value
                throughput_values.append(throughput)
            
            sorted_throughput_values = sorted(throughput_values)
            total_values = len(sorted_throughput_values)
            cumulative_probabilities = [(i + 1) / total_values for i in range(total_values)]
            
            for throughput, prob in zip(sorted_throughput_values, cumulative_probabilities):
                output_file.write(f"{throughput}\t{prob}\n")

