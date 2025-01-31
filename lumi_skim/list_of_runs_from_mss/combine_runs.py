with open("list_of_runs_from_3_to_10", 'w') as f:
    for i in range(3, 11):
        with open(f"list_of_runs_he{i}", 'r') as f1:
            lines = f1.readlines()
        for line in lines:
            line = line.strip()
            f.write(line+'\n')