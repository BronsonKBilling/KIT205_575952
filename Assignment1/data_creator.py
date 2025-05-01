def generate_ufc_dataset_to_file_10k():
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

def generate_ufc_dataset_to_file_100k():
    from itertools import product, islice
    import string
    import csv

    def generate_unique_names(n):
        """Generates n unique fighter names using combinations of uppercase letters."""
        letters = string.ascii_uppercase

        def name_generator():
            for length in range(1, 5):  # 1 to 4-letter combos
                for combo in product(letters, repeat=length):
                    yield ''.join(combo)

        gen = name_generator()
        return [f"{first} {last}" for first, last in zip(islice(gen, n), islice(gen, n, 2*n))]

    num_events = 25
    num_fighters = 100_000
    starting_event = 250

    print("Generating fighter names...")
    unique_fighters = generate_unique_names(num_fighters)
    print(f"Generated {len(unique_fighters)} unique fighters.")

    with open("ufc_dataset_100k.csv", "w", newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Event Number", "Fighter Name"])  # Header row

        for event in range(starting_event, starting_event - num_events, -1):
            print(f"Writing event {event}...")
            for fighter in unique_fighters:
                writer.writerow([event, fighter])

    print("Dataset written to ufc_dataset_100k.csv successfully.")

# Call the function
generate_ufc_dataset_to_file_100k()