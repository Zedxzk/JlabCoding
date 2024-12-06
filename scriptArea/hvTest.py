import re
itemToConfig = "v0set 1"

def get_hv_numbers(lines):
    pattern = re.compile(rf"ECAL:hv:(-?\d+):(-?\d+):({itemToConfig})([+-]?\d*(\.\d+)?([eE][+-]?\d+)?)")
    with open("NP.snap","w") as NP:
        with open("NN.snap","w") as NN:
            for line in lines:
                line = line.strip()
                match = pattern.search(line)
                if match:
                    col, row =  int(match.group(1)), int(match.group(2))
                    if col < 0 and row < 0:
                        NN.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 1000\n")
                        NP.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 0\n")
                    elif col < 0 and row > 0:
                        NN.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 0\n")
                        NP.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 1000\n")
                    else:
                        NN.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 0\n")
                        NP.write(f"ECAL:hv:{col}:{row}:{itemToConfig} 0\n")
                else:
                    NN.write(line+'\n')
                    NP.write(line+'\n')
                     


with open("ECAL_HV_29_Nov_2024_14_28_58.snap",'r') as file:
    lines = file.readlines()
    get_hv_numbers(lines)
