import re

# Data originally from 'https://www.kaggle.com/datasets/maksbasher/ufc-complete-dataset-all-events-1996-2024?resource=download'
# Dataset used is the Medium set

def main():
    dataset = ""
    new_dataset = ""
    line_values = []
    
    with open("medium_dataset.csv", "r", encoding="utf-8") as file:
        dataset = file.read()

    for line in dataset.splitlines():
        line_values = line.split(",")
        # ChatGPT was used to create the REGEX below
        if re.search(r'\bUFC\s\d{1,3}\b', line_values[0]):
            new_dataset = new_dataset + line_values[0].split(":")[0].split("UFC ")[1] + "," + line_values[5] + "\n"
            new_dataset = new_dataset + line_values[0].split(":")[0].split("UFC ")[1] + "," + line_values[6] + "\n"
    
    print(new_dataset)

main()
