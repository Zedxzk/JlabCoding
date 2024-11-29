import re

lines = [
    "ECAL:hv:9:8:v0set 1 1.0",
    "ECAL:hv:9:9:v0set 1 1.0",
    "ECAL:hv:10:5:v0set 1 1.0",
    "ECAL:hv:8:7:v0set 1 1.0"
]

def get_hv_numbers(line):
    match = re.search(r"hv:(\d+):(\d+)", line)
    if match:
        return int(match.group(1)), int(match.group(2))
    return None

sorted_lines = sorted(lines, key=get_hv_numbers)

for line in sorted_lines:
    print(line)
