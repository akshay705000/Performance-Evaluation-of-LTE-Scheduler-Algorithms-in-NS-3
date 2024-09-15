import os

# Get the current working directory
folder = os.getcwd()

# Create a new folder for the output files
output_folder = os.path.join(folder, "output")
os.makedirs(output_folder, exist_ok=True)

# Iterate over each file in the current directory
for filename in os.listdir(folder):
    # Check if the filename does not start with "throughput_"
    if not filename.startswith("throughput_"):
        # Construct the full path to the file
        filepath = os.path.join(folder, filename)
        
        # Check if the file is a regular file (not a directory)
        if os.path.isfile(filepath):
            # Execute the grep command to search for lines containing "throughput"
            # and write them to a new file inside the output folder
            output_filepath = os.path.join(output_folder, 'throughput_' + filename)
            os.system('grep -i "throughput" {} > {}'.format(filepath, output_filepath))

