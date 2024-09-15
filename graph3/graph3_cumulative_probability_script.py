import os

# Function to calculate cumulative probability
def calculate_cumulative_prob(data):
    total = sum(data)
    cumulative_prob = []
    cumulative_sum = 0
    for val in data:
        cumulative_sum += val / total
        cumulative_prob.append(round(cumulative_sum, 2))
    return cumulative_prob

# Function to read data from file and extract throughput values
def extract_throughput(file_path):
    throughputs = []
    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith("  Throughput:"):
                throughput = float(line.split()[1][:-3])
                throughputs.append(throughput)
    return throughputs

# Function to write data into a new file with cumulative probability
def write_to_file(file_path, throughputs, cumulative_probabilities):
    new_file_path = "cumulative_" + os.path.basename(file_path)
    with open(new_file_path, 'w') as file:
        for throughput, prob in zip(throughputs, cumulative_probabilities):
            file.write(f"{throughput}\t{prob}\n")
    print(f"New file '{new_file_path}' has been created with cumulative probabilities.")

# Main function
def main():
    files = [file for file in os.listdir() if file.startswith("throughput_")]
    for file_path in files:
        throughputs = extract_throughput(file_path)
        cumulative_probabilities = calculate_cumulative_prob(throughputs)
        write_to_file(file_path, throughputs, cumulative_probabilities)

# Execute the main function
main()

