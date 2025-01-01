import os
import re
from pprint import pprint


he_versions = [3, 4, 5, 6, 7, 8, 9, 10]
extra_info = "after_resubmission"
list_of_runs_dir = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss"
list_of_good_runs_dir = "/work/halld/home/zhikun/lumi_skim/goodRuns_after_resubmission"
list_of_good_runs_after_merging_dir = "/work/halld/home/zhikun/lumi_skim/goodRuns_merged_after_resubmission"
manual_check_log_dir = "/volatile/halld/home/test_lumi/"
list_of_error_files = "/work/halld/home/zhikun/lumi_skim/error_files"
evio_dir = "/mss/halld/RunPeriod-2022-08/recon/ver01/ps/"
jobs_to_resubmit_dir = "/work/halld/home/zhikun/lumi_skim/jobs_to_resubmit/"
list_of_files_reprocessed_dir = "/work/halld/home/zhikun/lumi_skim/list_of_files_reprocessed"
list_of_files_reprocessed_template = "list_Run{run_id}"
list_of_files_reprocessed_contents_template = "ps_{run_id}_{file_id}.evio"



pattern1 = re.compile(r"(Error: The directory (.+?(\d{6})) is empty.)")
pattern2 = re.compile(r"(Error: (.+?) (/w.+?/ps_(\d{6})_(\d{3}).log))")
pattern3 = re.compile(r"\('(\d+)', '(\d+|\*)'\)")

bad_runs_after_merging = set()
bad_runs = {}
overlap_truncated = {}
truncated_runs = {}
terminated_and_missing_files = {}
existing_evio_but_missing_log_files = {}
all_runs = []

num_truncated = 0
num_missing   = 0
num_crashed   = 0
num_terminated = 0
num_existing_evio_but_missing_log = 0
num_total     = 0
num_overlap_truncated = 0

def add_bad_run(key, value, dict = bad_runs):
    key = str(key)
    value = str(value)
    if key in dict.keys():
        dict[key].append(value)
    else:
        dict[key] = [value]  # Use list notation to create a new list with the value


for i in he_versions:
    with open(os.path.join(list_of_good_runs_dir, f'goodRuns_he{i}.txt'),'w') as goodRuns:
        list_of_runs = os.path.join(list_of_runs_dir, f"list_of_runs_he{i}")
        with open(list_of_runs,'r') as f1:
            runs = [item.strip() for item in f1.readlines()]
        all_runs.append(runs)
        with open(f"res_new_he{i}_{extra_info}.txt") as f1:
            errors_new = [ item.strip() for item in f1.readlines() if  item.strip() != ""]
        with open(f"res_new_he{i}_{extra_info}.txt") as f2:
            errors_old = [ item.strip() for item in f2.readlines() if  item.strip() != ""]
        manual_check_log_path = os.path.join(manual_check_log_dir, f"manual_check_he{i}_after_resubmission.log")

        
        with open(manual_check_log_path,'r') as file:
            for line in file:
                matches = re.findall(pattern3, line)
                for match in matches:
                    # 添加第一个数字到集合中
                    bad_runs_after_merging.add(match[0])

        with open(os.path.join(list_of_good_runs_after_merging_dir, f'goodRuns_he{i}.txt'),'w') as goodRuns_merged:
            for run in runs:
                if run not in bad_runs_after_merging:
                    goodRuns_merged.write(f"{run}\n")


        # print(runs)
        for error in errors_new:
            match1 = pattern1.search(error)
            if match1:
                first_number = str(match1.group(3))
                add_bad_run(first_number, "*")
                if first_number in runs:
                    # print(first_number)
                    runs.remove(str(first_number))

            match2 = pattern2.search(error)
            # print()
            if match2:
                first_number = match2.group(4)
                second_number = match2.group(5)
                add_bad_run(first_number, second_number)
                if first_number in runs:
                    # print(first_number)
                    runs.remove(str(first_number))
            
            if "Log file not found" in error :
                add_bad_run(first_number, second_number, terminated_and_missing_files)
                evio_path = os.path.join(evio_dir, first_number, f"ps_{first_number}_{second_number}.evio")
                if os.path.exists(evio_path):
                    num_existing_evio_but_missing_log += 1
                    add_bad_run(first_number, second_number, existing_evio_but_missing_log_files)
                num_missing += 1
            elif  "No events in file" in error:
                add_bad_run(first_number, second_number, terminated_and_missing_files)
                num_terminated += 1
            elif "words_left_in_file" in error:
                add_bad_run(first_number,second_number,  truncated_runs)
                num_truncated += 1
            elif "crash" in error:
                num_crashed += 1
            else:
                exit(f"Unmatch error type:{error}")

        pprint(bad_runs)
        for run in runs:
            goodRuns.write(f"{run}\n")

            
num_total = num_missing + num_crashed + num_truncated + num_terminated

with open(f"files_need_to_be_reprocessed_{extra_info}.txt", 'w') as f:
    msg = '\n'.join([
        "***********************************************",
        f"Total error nums    = {num_total}",
        f"Truncated files num = {num_truncated}",
        f"Crashed files num   = {num_crashed}",
        f"Terminated files num= {num_terminated}",
        f"Missing logs nums   = {num_missing}",
        f"Missing log but existing evio = {num_existing_evio_but_missing_log}",
        "***********************************************",
        "\n\n"
    ])

    f.write(msg)
    for key, value in bad_runs.items():
        
        f.write(key+'\n')
        f.write(f"{value}")
        f.write("\n\n\n")
    f.write(msg)
        
num_total = num_missing + num_terminated
with open(f"files_need_to_be_resubmitted_{extra_info}.txt", 'w') as f:
    msg = '\n'.join([
        "***********************************************",
        f"Total error nums    = {num_total}",
        f"Terminated files num= {num_terminated}",
        f"Missing logs nums   = {num_missing}",
        f"Missing log but existing evio = {num_existing_evio_but_missing_log}\n"
        "***********************************************",
        "\n\n"
    ])

    f.write(msg)
    for key, value in terminated_and_missing_files.items():
        
        f.write(key+'\n')
        f.write(f"{value}")
        f.write("\n\n\n")
    f.write(msg)

with open(f"list_of_resubmission_{extra_info}", "w") as fi:
    with open(f"existing_evio_but_missing_log_{extra_info}.txt", 'w') as f:
        for key, value in existing_evio_but_missing_log_files.items():
            fi.write(f"{key}\n")
            with open(os.path.join(jobs_to_resubmit_dir, f"list_Run{key}"),"w") as file:
                for file_id in value:
                    evio_path = os.path.join(evio_dir, key, f"ps_{key}_{file_id}.evio")
                    f.write(f"{evio_path}\n")
                    file.write(f"ps_{key}_{file_id}.evio\n")
            f.write(key+'\n')
            f.write(f"{value}\n")

            f.write("\n\n")
        f.write(msg)


with open(f"truncated_runs_{extra_info}", "w") as fi:
    msg = '\n'.join([
        "****************************************",
        f"Truncated runs in total = {num_truncated}",
        "****************************************\n"
    ])
    fi.write(msg)
    for key, value in truncated_runs.items():
            fi.write(f"{key}\n")
            fi.write(f"{value}\n\n")
    fi.write(msg)

with open("stdout.log","r") as f :
    pattern4 = r'stdout\.(\d+)_(\d+)\.out'
    # 查找所有匹配项 
    matches = re.findall(pattern4, f.read())
    for match in matches:
        if str(match[0]) in truncated_runs.keys():
            # print(f"overlaped run id = {match[0]}")
            if str(match[1]) in truncated_runs[str(match[0])]:
                add_bad_run(match[0], match[1], overlap_truncated)
                num_overlap_truncated += 1

with open("overlap_trunated", "w") as fi:
    msg = "\n".join([
        "****************************************",
        f"Overlaped truncated runs  in total = {num_overlap_truncated}",
        "****************************************\n"
    ])
    fi.write(msg)
    for key, value in overlap_truncated.items():
            fi.write(f"{key}\n")
            fi.write(f"{value}\n\n")
    fi.write(msg)


with open("list_of_runs_reprocessed","w") as f:
    if not os.path.exists(list_of_files_reprocessed_dir):
        os.makedirs(list_of_files_reprocessed_dir)
    for key, value in bad_runs.items():
        f.write(key+"\n")
        list_of_files_reprocessed_path = os.path.join(list_of_files_reprocessed_dir, list_of_files_reprocessed_template)
        list_of_files_reprocessed_path = list_of_files_reprocessed_path.format(run_id=key)
        print(list_of_files_reprocessed_path)
        with open(list_of_files_reprocessed_path,"w") as f1:
            for file_id in value:
                list_of_files_reprocessed_contents = list_of_files_reprocessed_contents_template.format(run_id=key, file_id=file_id)
                f1.write(list_of_files_reprocessed_contents+"\n")
        # with open()


