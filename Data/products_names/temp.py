# Read the order file into a list
with open('most_popular.txt', 'r') as f:
    order = [line.strip() for line in f]

# Read the data file into a list
with open('reduced.txt', 'r') as f:
    data = [line.strip() for line in f]

# Create a dictionary to store the data lines for quick lookup
data_dict = {line: line for line in data}

# Sort the data based on the order from the order file
sorted_data = [data_dict[line] for line in order if line in data_dict]

# Save the sorted result to a new file
with open('sorted_data_file.txt', 'w') as f:
    for line in sorted_data:
        f.write(line + '\n')

print("Data sorted and saved to 'sorted_data_file.txt'")
