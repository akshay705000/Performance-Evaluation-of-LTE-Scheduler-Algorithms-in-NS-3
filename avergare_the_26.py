import os
import re

# Function to read files, extract average throughput, and accumulate it for each scheduler
def process_files(folder, pattern):
    # Dictionary to store scheduler names and corresponding cumulative throughputs
    scheduler_throughputs = {}

    # Regex pattern to match filenames with the specified pattern
    filename_pattern = re.compile(fr'^throughput_([^_]+)_{pattern}_(\d+\.?\d*).txt$')

    # Iterate over files in the folder
    for filename in os.listdir(folder):
        # Check if filename matches the pattern
        match = filename_pattern.match(filename)
        if match:
            scheduler_name = match.group(1)
            file_path = os.path.join(folder, filename)
            
            # Read the file and extract average throughput
            with open(file_path, 'r') as file:
                for line in file:
                    if line.startswith("Average throughput="):
                        throughput = float(line.split('=')[1].strip().split()[0])
                        # Accumulate throughput for the corresponding scheduler
                        if scheduler_name in scheduler_throughputs:
                            scheduler_throughputs[scheduler_name] += throughput
                        else:
                            scheduler_throughputs[scheduler_name] = throughput
    
    return scheduler_throughputs

# Function to calculate average throughput for each scheduler
def calculate_average_throughput(scheduler_throughputs, pattern):
    average_throughputs = {}
    for scheduler, throughput_sum in scheduler_throughputs.items():
        num_files = len([fname for fname in os.listdir() if f'throughput_{scheduler}_{pattern}' in fname])
        if num_files == 0:
            average_throughputs[scheduler] = 0  # Set average throughput to 0 if no files found
        else:
            average_throughputs[scheduler] = throughput_sum / num_files
    return average_throughputs

# Function to write average throughputs to a new file
def write_to_file(average_throughputs, pattern):
    with open(f"average_throughputs_{pattern}.txt", "w") as file:
        for scheduler, throughput in average_throughputs.items():
            file.write(f"{scheduler}\t{throughput:.2E}\n")
    print(f"Average throughputs have been written to 'average_throughputs_{pattern}.txt'")

def main():
    folder = os.getcwd()
    patterns = ["full_0", "full_5"]

    for pattern in patterns:
        scheduler_throughputs = process_files(folder, pattern)
        average_throughputs = calculate_average_throughput(scheduler_throughputs, pattern)
        write_to_file(average_throughputs, pattern)

if __name__ == "__main__":
    main()

