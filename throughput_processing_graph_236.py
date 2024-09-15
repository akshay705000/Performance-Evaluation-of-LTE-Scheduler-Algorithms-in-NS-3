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
            # Execute the grep command to search for lines containing "throughput"
            # and write them to a new file with "stat3_" prepended to the filename
            os.system('grep -i "throughput" {} > {}'.format(filepath, os.path.join(folder, 'throughput_' + filename)))

