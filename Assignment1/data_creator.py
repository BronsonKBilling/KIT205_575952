def generate_ufc_dataset_to_file():
    from itertools import product, count
    import string
    import csv

    def generate_unique_names(n):
        letters = string.ascii_uppercase
        name_parts = []
        for length in count(1):
            for combo in product(letters, repeat=length):
                name_parts.append(''.join(combo))
                if len(name_parts) >= n * 2:  # Each full name needs two parts
                    return [f"{name_parts[i]} {name_parts[i + 1]}" for i in range(0, n * 2, 2)]

    unique_fighters = generate_unique_names(10000)

    with open("ufc_dataset.csv", "w", newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Event Number", "Fighter Name"])  # Header row

        for event in range(250, 0, -1):
            for fighter in unique_fighters:
                writer.writerow([event, fighter])

    print("Dataset written to ufc_dataset.csv successfully.")

# Call the function
generate_ufc_dataset_to_file()