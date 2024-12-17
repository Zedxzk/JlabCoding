import re
itemToConfig = "v0set 1"

def get_hv_numbers(lines):
    pattern = re.compile(rf"ECAL:hv:(-?\d+):(-?\d+):({itemToConfig})([+-]?\d*(\.\d+)?([eE][+-]?\d+)?)")
    with open("N.snap","w") as N:
            for line in lines:
                line = line.strip()
                match = pattern.search(line)
                if match:
                    col, row =  int(match.group(1)), int(match.group(2))
                    if col < 0:
                        N.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 1040\n")
                    else:
                        N.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 0\n")
                else:
                    N.write(line+'\n')
                     


with open("ECAL_HV_29_Nov_2024_14_28_58.snap",'r') as file:
    lines = file.readlines()
    get_hv_numbers(lines)
