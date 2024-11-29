import subprocess
import os
import glob

he_version = "he3"
log_base_dir = "/volatile/halld/home/test_lumi/log/"
list_of_runs = f'/work/halld3/home/somov/lumi_skim/list_of_runs_primex3/list_of_runs_{he_version}'
evio_list_base_dir = "/work/halld3/home/somov/lumi_skim/lumi_primex3/individual"


def get_lines_from_file(filepath):
    """Read lines from a file and return them as a list."""
    with open(filepath, 'r') as f:
        return [line.strip() for line in f.readlines()]

def process_file(file):
    """Process a single file, extract events and log information."""
    my_file = file.replace('run_', 'Run0')
    
    evt_total = 0
    n = 0
    n1 = 0

    print(my_file)

    # Construct the list of filenames from the log directory
    evio_list_path = os.path.join(evio_list_base_dir, f"list_Run{my_file}")
    log_files = get_lines_from_file(evio_list_path)


    for file1 in log_files:
        my_file_log = file1.replace('ps.', '').replace('hd_rawdata', 'ps').replace('evio', 'log').replace("ps_ps_","ps_")
        log_path = os.path.join(log_base_dir, f'Run{my_file}/{my_file_log}')

        n += 1

        try:
            with open(log_path, 'r',errors="ignore") as log_file:
                log_contents = log_file.read()

                bad_evt = [line for line in log_contents.splitlines() if 'words_left_in_file' in line]

                if bad_evt:
                    print(f"\033[1;34m  '{log_path}' \033[0m")

                evt = [line for line in log_contents.splitlines() if 'Nevents:' in line]

                if evt:
                    evt_count = int(evt[0].split(':')[1].strip())  # Get the event number
                    evt_total += evt_count
                    n1 += 1
                else:
                    print(f"\033[1;31m No events in file  '{log_path}' \033[0m")
                
        except FileNotFoundError:
            print(f"File not found: {log_path}")

    print(f" Run   Files  Good      Events    {my_file}  {n}  {n1}  {evt_total/ 1000000:.2f}")
    print("   ")

def main():
    # Get the list of files. This replaces the tcsh foreach loop with the list of files to process.
    files_to_process = get_lines_from_file(list_of_runs)

    for file in files_to_process:
        process_file(file)

if __name__ == "__main__":
    main()