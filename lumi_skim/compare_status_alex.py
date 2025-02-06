import os
import re
from pprint import pprint
import process_error_messages
# from  findList_function_definition import add_bad_run, create_dirs,  write_files

# **********************   User Setting Starts ************************
# ********************** Input Setting Starts ************************
he_versions = ["he3", "he4", "he5", "he6", "he7", "he8", "he9", "he10"]
# he_versions = ["he2"]
extra_info = "after_reprocessing"

# list_of_runs_dir = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss"
list_of_runs_path_template = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/list_of_runs_{he_version}"
list_of_files_dir = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss"
# evio_dir = "/mss/halld/RunPeriod-2022-08/recon/ver01/ps/"
evio_name_template = "ps_{run_id}_{file_id}.evio"
evio_path_template = "/mss/halld/RunPeriod-2022-08/recon/ver01/ps/ps_{run_id}_{file_id}.evio"
res_dir_1 = "/work/halld/home/zhikun/lumi_skim/scan_res_dir/"
res_dir_2 = "/work/halld/home/zhikun/lumi_skim/scan_res_dir/"
res_file_name_template_1 = "res_new_{he_version}_{extra_info}.txt"
res_file_name_template_2 = "res_old_{he_version}_{extra_info}.txt"

# ********************** Input Setting Ends ************************


# ********************** Output Settings Starts ************************
list_of_runs_dir  = "/work/halld/home/zhikun/lumi_skim/list_of_runs/he3_to_10"
list_of_runs_name_template = "list_of_runs_{he_version}"
list_of_files_dir = "/work/halld/home/zhikun/lumi_skim/list_of_files/he3_to_10"
list_of_seperate_good_runs_before_merging_dir         = "/work/halld/home/zhikun/lumi_skim/goodRuns_after_resubmission/"
list_of_seperate_good_runs_after_merging_dir          = "/work/halld/home/zhikun/lumi_skim/list_of_good_runs_after_merging/"
list_of_seperate_bad_runs_before_merging_dir          = "/work/halld/home/zhikun/lumi_skim/list_of_bad_runs_before_merging/"
list_of_seperate_bad_runs_after_merging_dir           = "/work/halld/home/zhikun/lumi_skim/list_of_bad_runs_after_merging/"

# manual_check_log_dir = "/volatile/halld/home/test_lumi/"

list_of_error_files = "/work/halld/home/zhikun/lumi_skim/error_files"
evio_dir = "/mss/halld/RunPeriod-2022-08/recon/ver01/ps/"
list_of_files_to_reprocess_dir = "/work/halld/home/zhikun/lumi_skim/list_of_files_reprocessed"
jobs_to_resubmit_dir = "/work/halld/home/zhikun/lumi_skim/jobs_to_resubmit/"
visualized_dir = "/work/halld/home/zhikun/lumi_skim/visualized_dir"

list_of_files_name_template = "list_Run{run_id}"
good_runs_file_name_template = "goodRuns_{he_version}_{extra_info}.txt"
list_of_bad_runs_after_merging_name_template = "bad_runs_{he_version}_{extra_info}.txt"
list_of_bad_runs_before_merging_name_template = "bad_runs_{he_version}_{extra_info}.txt"
# ********************** Output Setting Ends ************************

# **********************   User Setting Ends ************************




pattern1 = re.compile(r"(Error: The directory\s+(.+?)(\d{6})\s+is empty.)")
pattern2 = re.compile(r"(Error: (.+?) (/w.+?/ps_(\d{6})_(\d{3}).log))")
pattern3 = re.compile(r"\('(\d+)', '(\d+|\*)'\)")

all_bad_runs_run_id_after_merging = set()
part_of_bad_runs_run_id_after_merging = set()
part_of_bad_runs_run_id_source_1 = set()
all_good_runs_run_id_after_merging = set()

all_bad_runs_of_source_1 = {}
# bad_runs_after_merging = {}
overlap_truncated = {}

empty_runs  = {}
truncated_runs_and_files = {}
existing_evio_but_missing_log_runs_and_files = {}
missing_evio_runs_and_files = {}
terminated_runs_and_files = {}
crashed_runs_and_files = {}

terminated_runs_and_files = {}
all_runs = []

num_truncated = 0
num_missing_evio   = 0
num_crashed   = 0
num_terminated = 0
num_existing_evio_but_missing_log = 0
num_total     = 0
num_overlap_truncated = 0
num_empty_run = 0


error_types = ["truncated", "missing_evio", "existing_evio_but_missing_log", "crashed", "terminated", "empty_folder"]
error_dicts = [truncated_runs_and_files, missing_evio_runs_and_files, existing_evio_but_missing_log_runs_and_files, crashed_runs_and_files, terminated_runs_and_files, empty_runs]

def add_bad_run(key, value, dict):
    key = str(key)
    value = str(value)
    if key in dict.keys():
        dict[key].append(value)
    else:
        dict[key] = [value]  # Use list notation to create a new list with the value


def create_dirs(list_of_error_types):
    for error_type in list_of_error_types:
        dir1 =os.path.join(list_of_runs_dir, error_type)
        dir2 =os.path.join(list_of_files_dir, error_type)
        if not os.path.exists(dir1):
            os.makedirs(dir1)
        if not os.path.exists(dir2):
            os.makedirs(dir2)

def write_files(list_of_error_types,  error_dicts):
    for error_type, error_dict in zip(list_of_error_types, error_dicts):
        dir1 =os.path.join(list_of_runs_dir, error_type)
        dir2 =os.path.join(list_of_files_dir, error_type)
        list_of_runs_path = os.path.join(dir1, f"list_of_runs_all")
        with open(list_of_runs_path, "w") as f1:
            for key, value in error_dict.items():
                run_id = key
                f1.write(run_id+"\n")
                list_of_files_path = os.path.join(dir2,list_of_files_name_template).format(run_id = run_id)
                with open(list_of_files_path, "w") as f2:
                    for file_id in value:
                        f2.write(evio_name_template.format(run_id=run_id, file_id=file_id))
                        f2.write("\n")
        if len(all_runs) != len(he_versions):
            exit("Error: length of he_versions does not equal to length of all_runs")
        for index in range(len(all_runs)):
            he_version  = he_versions[index]
            list_of_runs_path = os.path.join(dir1, list_of_runs_name_template).format(he_version=he_version)
            with open(list_of_runs_path, "w") as f3:
                for run_id in all_runs[index]:
                    if run_id in error_dict:
                        f3.write(run_id)
                        f3.write("\n")

        
                        

def write_user_define_error_type(dicts):
    user_define_error_type_dict = {}
    for error_dict in dicts:
        user_define_error_type_dict.update(error_dict)
    create_dirs(["user_define_error_type"])
    dir1 =os.path.join(list_of_runs_dir, "user_define_error_type")
    dir2 =os.path.join(list_of_files_dir, "user_define_error_type")
    list_of_runs_path = os.path.join(dir1, f"list_of_runs_all")
    with open(list_of_runs_path, "w") as f1:
        for key, value in user_define_error_type_dict.items():
            run_id = key
            f1.write(run_id+"\n")
            list_of_files_path = os.path.join(dir2,list_of_files_name_template).format(run_id = run_id)
            with open(list_of_files_path, "w") as f2:
                for file_id in value:
                    f2.write(evio_name_template.format(run_id=run_id, file_id=file_id))
                    f2.write("\n")

        if len(all_runs) != len(he_versions):
            exit("Error: length of he_versions does not equal to length of all_runs")
        for index in range(len(all_runs)):
            he_version  = he_versions[index]
            list_of_runs_path = os.path.join(dir1, list_of_runs_name_template).format(he_version=he_version)
            with open(list_of_runs_path, "w") as f3:
                for run_id in all_runs[index]:
                    if run_id in user_define_error_type_dict:
                        f3.write(run_id)
                        f3.write("\n")


for he_version in he_versions:
    part_of_bad_runs_run_id_after_merging = set()
    part_of_bad_runs_run_id_source_1 = set()
    if not os.path.exists(list_of_seperate_good_runs_before_merging_dir):
        os.makedirs(list_of_seperate_good_runs_before_merging_dir)
    if not os.path.exists(list_of_seperate_bad_runs_after_merging_dir):
        os.makedirs(list_of_seperate_bad_runs_after_merging_dir)
    with open(os.path.join(list_of_seperate_good_runs_before_merging_dir, good_runs_file_name_template.format(he_version=he_version,extra_info=extra_info)),'w') as goodRuns:
        # with open(os.path.join(list_of_good_runs_dir, f'goodRuns_{i}.txt'),'w') as goodRuns:
        list_of_runs = os.path.join(list_of_runs_path_template.format(he_version=he_version, extra_info=extra_info))
        with open(list_of_runs,'r') as f1:
            runs = [str(item.strip()) for item in f1.readlines()]
        all_runs.append(runs.copy())


        resFile_source_1 = os.path.join(res_dir_1, res_file_name_template_1).format(he_version=he_version, extra_info=extra_info)
        resFile_source_2 = os.path.join(res_dir_2, res_file_name_template_2).format(he_version=he_version, extra_info=extra_info)


        with open(resFile_source_1, "r") as f1:
            errors_new = [ item.strip() for item in f1.readlines() if  item.strip() != ""]
        # with open(f"text_files/res_new_{i}_{extra_info}.txt") as f2:
        #     errors_old = [ item.strip() for item in f2.readlines() if  item.strip() != ""]

        formated_errors_source_1, formated_errors_source_2 = process_error_messages.check_all(resFile_source_1, resFile_source_2)
        # pprint(formated_errors_source_1)
        # pprint(formated_errors_source_2)

        # for key in formated_errors_source_1:
        #     print(f"new: {key}" )
        # print(formated_errors_source_2)
        # for key in formated_errors_source_2:
        #     print(f"old: {key}" )
        part_of_bad_runs_after_merging = process_error_messages.compare_result(formated_errors_source_1, formated_errors_source_2)
        list_of_bad_runs_after_merging_path = os.path.join(list_of_seperate_bad_runs_after_merging_dir, list_of_bad_runs_after_merging_name_template)
        list_of_bad_runs_after_merging_path = list_of_bad_runs_after_merging_path.format(he_version=he_version, extra_info=extra_info)

        for key in  part_of_bad_runs_after_merging:
            part_of_bad_runs_run_id_after_merging.add(key[0])
            all_bad_runs_run_id_after_merging.add(key[0])
            
        # pprint(temp_runs)
                # input(f"Shared problem: {key}")
                # bad_runs_after_merging.add(key[0])

        # pprint(all_bad_runs_run_id_after_merging)
        # input(f"he_version =  {i}, length of bad runs = {len(bad_runs_after_merging)}")

        # manual_check_log_path = os.path.join(manual_check_log_dir, f"manual_check_{he_version}_after_resubmission.log")
        
        # with open(manual_check_log_path,'r') as file:
        #     for line in file:
        #         matches = re.findall(pattern3, line)
        #         for match in matches:
        #             # 添加第一个数字到集合中
        #             bad_runs_after_merging.add(match[0])

        if not os.path.exists(list_of_seperate_good_runs_after_merging_dir):
            os.makedirs(list_of_seperate_good_runs_after_merging_dir)
        with open(os.path.join(list_of_seperate_good_runs_after_merging_dir, f'goodRuns_{he_version}.txt'),'w') as goodRuns_merged:
            for run in runs:
                if run not in all_bad_runs_run_id_after_merging:
                    goodRuns_merged.write(f"{run}\n")


        # print(runs)
        if not os.path.exists(list_of_seperate_bad_runs_before_merging_dir):
            os.makedirs(list_of_seperate_bad_runs_before_merging_dir)
        list_of_bad_runs_before_merging_path = os.path.join(list_of_seperate_bad_runs_before_merging_dir, list_of_bad_runs_before_merging_name_template).format(he_version=he_version, extra_info=extra_info)
            
        for error in errors_new:
            match1 = pattern1.search(error)
            match2 = pattern2.search(error)

            if match1:
                first_number = str(match1.group(3))
                second_number = None
                add_bad_run(first_number, "*", all_bad_runs_of_source_1)
                part_of_bad_runs_run_id_source_1.add(first_number)
                if first_number in runs:
                    # print(first_number)
                    runs.remove(str(first_number))

            # print()
            elif match2:
                first_number = match2.group(4)
                second_number = match2.group(5)
                add_bad_run(first_number, second_number, all_bad_runs_of_source_1)
                part_of_bad_runs_run_id_source_1.add(first_number)
                if first_number in runs:
                    # print(first_number)
                    runs.remove(str(first_number))
            
            else:
                exit("Can not match error type:\n" + error + "\n")
            

            
            if "Log file not found" in error :
                evio_path = os.path.join(evio_dir, first_number, f"ps_{first_number}_{second_number}.evio")
                if os.path.exists(evio_path):
                    num_existing_evio_but_missing_log += 1
                    add_bad_run(first_number, second_number, existing_evio_but_missing_log_runs_and_files)
                else:
                    num_missing_evio += 1
                    add_bad_run(first_number, second_number, missing_evio_runs_and_files)
            elif  "No events in file" in error:
                add_bad_run(first_number, second_number, terminated_runs_and_files)
                num_terminated += 1
            elif "words_left_in_file" in error:
                add_bad_run(first_number,second_number,  truncated_runs_and_files)
                num_truncated += 1
            elif "crash" in error:
                add_bad_run(first_number,second_number,  crashed_runs_and_files)
                num_crashed += 1
            elif "is empty" in error:
                num_empty_run += 1
                add_bad_run(first_number, "*", empty_runs)
            else:
                exit(f"Unmatch error type:\n{error}")


        for run in runs:
            goodRuns.write(f"{run}\n")
        part_of_bad_runs_run_id_after_merging = sorted(part_of_bad_runs_run_id_after_merging)
        part_of_bad_runs_run_id_source_1 = sorted(part_of_bad_runs_run_id_source_1)
        with open(list_of_bad_runs_after_merging_path, "w") as f2:
            for value in part_of_bad_runs_run_id_after_merging:
                f2.write(value)
                f2.write("\n")
        with open(list_of_bad_runs_before_merging_path, "w") as f3:
            for value in part_of_bad_runs_run_id_source_1:
                f3.write(value)
                f3.write("\n")

            
num_total = num_missing_evio + num_crashed + num_truncated + num_terminated + num_empty_run + num_existing_evio_but_missing_log

print("All bad runs and files of source 1 include:")
pprint(all_bad_runs_of_source_1)
print("All bad runs and files of source 1 are include above")



if not os.path.exists(visualized_dir):
    os.makedirs(visualized_dir)

if not os.path.exists(list_of_runs_dir):
    os.makedirs(list_of_runs_dir)

if not os.path.exists(list_of_files_dir):
    os.makedirs(list_of_files_dir)

with open(os.path.join(visualized_dir, f"files_of_all_errors_{extra_info}.txt"), 'w') as f:
    msg = '\n'.join([
        "***********************************************",
        f"Total error nums              = {num_total}",
        f"Empty runs                    = {num_empty_run}",
        f"Truncated files num           = {num_truncated}",
        f"Crashed files num             = {num_crashed}",
        f"Terminated files num          = {num_terminated}",
        f"Missing evio nums             = {num_missing_evio}",
        f"Missing log but existing evio = {num_existing_evio_but_missing_log} (jobs did not start)",
        "***********************************************",
        "\n\n"
    ])

    f.write(msg)
    for key, value in all_bad_runs_of_source_1.items():
        
        f.write(key+'\n')
        f.write(f"{value}")
        f.write("\n\n\n")
    f.write(msg)
        
# num_total = num_existing_evio_but_missing_log + num_terminated
# with open(os.path.join(visualized_dir, f"files_of_missing_log_{extra_info}.txt"), 'w') as f:
#     msg = '\n'.join([
#         "***********************************************",
#         f"Total error nums    = {num_total}",
#         f"Terminated files num= {num_terminated} (may need to resubmit)",
#         f"Missing log but existing evio = {num_existing_evio_but_missing_log}\n"
#         "***********************************************",
#         "\n\n"
#     ])

#     f.write(msg)
#     for key, value in terminated_and_missing_files.items():
#         f.write(key+'\n')
#         f.write(f"{value}")
#         f.write("\n\n\n")
#     f.write(msg)


with open(os.path.join(visualized_dir, f"files_of_existing_evio_but_missing_log_{extra_info}.txt"), "w") as f1:
    msg = '\n'.join([
        "***********************************************",
        f"Missing log but existing evio = {num_existing_evio_but_missing_log}\n"
        "***********************************************",
        "\n\n"
    ])
    f1.write(msg + "\n\n")
    with open(os.path.join(list_of_runs_dir, f"list_of_resubmission_{extra_info}"), "w") as fi:
        for key, value in existing_evio_but_missing_log_runs_and_files.items():
            f1.write(f"{key}\n")
            fi.write(f"{key}\n")
            with open(os.path.join(jobs_to_resubmit_dir, f"list_Run{key}"),"w") as file:
                for file_id in value:
                    evio_name_template.format(run_id = key, file_id=file_id)
                    file.write(f"ps_{key}_{file_id}.evio\n")
            f1.write(f"{value}\n")


with open(os.path.join(visualized_dir,f"files_of_truncated_runs_{extra_info}.txt"), "w") as fi:
    msg = '\n'.join([
        "****************************************",
        f"Truncated runs in total = {num_truncated}",
        "****************************************\n"
    ])
    fi.write(msg)
    for key, value in truncated_runs_and_files.items():
            fi.write(f"{key}\n")
            fi.write(f"{value}\n\n")
    fi.write(msg)

with open(os.path.join(visualized_dir,f"files_of_crashed_runs_{extra_info}.txt"), "w") as fi:
    msg = '\n'.join([
        "****************************************",
        f"Crashed runs in total = {num_crashed}",
        "****************************************\n"
    ])
    fi.write(msg)
    for key, value in crashed_runs_and_files.items():
            fi.write(f"{key}\n")
            fi.write(f"{value}\n\n")
    fi.write(msg)


with open(os.path.join(visualized_dir,f"files_of_terminated_runs_{extra_info}.txt"), "w") as fi:
    msg = '\n'.join([
        "****************************************",
        f"Terminated runs in total = {num_crashed}",
        "****************************************\n"
    ])
    fi.write(msg)
    for key, value in terminated_runs_and_files.items():
            fi.write(f"{key}\n")
            fi.write(f"{value}\n\n")
    fi.write(msg)


# with open("stdout.log","r") as f :
#     pattern4 = r'stdout\.(\d+)_(\d+)\.out'
#     # 查找所有匹配项 
#     matches = re.findall(pattern4, f.read())
#     for match in matches:
#         if str(match[0]) in truncated_runs_and_files.keys():
#             # print(f"overlaped run id = {match[0]}")
#             if str(match[1]) in truncated_runs_and_files[str(match[0])]:
#                 add_bad_run(match[0], match[1], overlap_truncated)
#                 num_overlap_truncated += 1


        

create_dirs(error_types)
write_files(error_types, error_dicts)
# with open(os.path.join(visualized_dir,f"overlap_trunated_{extra_info}"), "w") as fi:
#     msg = "\n".join([
#         "****************************************",
#         f"Overlaped truncated runs  in total = {num_overlap_truncated}",
#         "****************************************\n"
#     ])
#     fi.write(msg)
#     for key, value in overlap_truncated.items():
#             fi.write(f"{key}\n")
#             fi.write(f"{value}\n\n")
#     fi.write(msg)


with open(os.path.join(list_of_runs_dir, f"list_of_all_errors_of_source_1_{extra_info}"),"w") as f:
    if not os.path.exists(list_of_files_to_reprocess_dir):
        os.makedirs(list_of_files_to_reprocess_dir)
    for key, value in all_bad_runs_of_source_1.items():
        
        f.write(key+"\n")
        list_of_files_reprocessed_path = os.path.join(list_of_files_to_reprocess_dir, list_of_files_name_template)
        list_of_files_reprocessed_path = list_of_files_reprocessed_path.format(run_id=key)
        # print(list_of_files_reprocessed_path)
        with open(list_of_files_reprocessed_path,"w") as f1:
            for file_id in value:
                list_of_files_reprocessed_contents = evio_name_template.format(run_id=key, file_id=file_id)
                f1.write(list_of_files_reprocessed_contents+"\n")
        # with open()
        
with open(os.path.join(list_of_runs_dir, f"list_of_all_good_runs_after_merging_{extra_info}"), "w") as f:
    # print(len(all_runs))
    for index in range(len(all_runs)):
        # print(index)
        temp = all_runs[index]
        # print(temp)
        msg = ""
        num_good_runs = 0
        for he_version in temp:
            if he_version not in all_bad_runs_run_id_after_merging:
                msg += f"{he_version}\n"
                num_good_runs += 1
        f.write(f"\nversion = {he_versions[index]}, there are(is) {num_good_runs} good runs out of totally {len(temp)} runs:\n")
        f.write(msg)



with open(os.path.join(list_of_runs_dir, f"list_of_all_bad_runs_after_merging_{extra_info}"), "w") as f:
    # print(len(all_runs))
    for index in range(len(all_runs)):
        # print(index)
        temp = all_runs[index]
        # print(temp)
        msg = ""
        num_good_runs = 0
        for he_version in temp:
            if he_version in all_bad_runs_run_id_after_merging:
                msg += f"{he_version}\n"
                num_good_runs += 1
        f.write(f"\nversion = {he_versions[index]}, there are(is) {num_good_runs} bad runs out of totally {len(temp)} runs:\n")
        f.write(msg)



write_user_define_error_type([terminated_runs_and_files, existing_evio_but_missing_log_runs_and_files])

with open("compare_status.txt", "r") as f:
    all_files_to_compare = [ i.split("_") for i in f.readlines()    ]


# for error_type, error_dict in zip(error_types, error_dicts):
#     print(f"error Type: {error_type}")
#     pprint( error_dict)
# input()


with open("compare_results.txt", "w")  as f:
    for run, file in  all_files_to_compare:
        find_type = False
        run = str(run)
        file = f"{int(file):03d}"
        # print(f"{run}  {file} ")
        for error_type, error_dict in zip(error_types, error_dicts):
            # pprint(f"error Type: {error_type}", error_dict)
            # if (run, file) in error_dict.items():
            if run in error_dict:
                # print(f"run {run} in dict, type = {error_type}")
                if file in error_dict[run]:
                    f.write(f"{run}  {file}  {error_type}\n")
                    find_type = True
                    break
            else:
                pass
        if not find_type:
            print(f"Can not find {run}  {file} ")
