import process_error_messages
import os
from pprint import pprint
import subprocess
he_versions = ["reprocessed"]
extraInfo = "for_exclusion"

num_excluded_files = 0
delete_files = True
# delete_files = False

# # Paths Setup
rootDir1 = "/volatile/halld/home/zhikun/ver05_lumi/"
logDir1 = "/work/halld3/home/somov/lumi_skim/lumi_primex3/individual/log"

res_path_1_template = "/work/halld/home/zhikun/lumi_skim/exclusion_dir/res_sasha_{he_version}_{extraInfo}.txt"

# rootDir2 = "/volatile/halld/home/zhikun/merged_new_runs"
logDir2 = "/work/halld3/home/somov/lumi_skim/lumi_primex3_new/individual/log"
res_path_2_template = "/work/halld/home/zhikun/lumi_skim/exclusion_dir/res_alex_{he_version}_{extraInfo}.txt"

files_to_delete_template = "/volatile/halld/home/zhikun/merged_new_runs/Run{run_id}/ps_{run_id}_{file_id}.root"


list_of_runs_from_mss_dir = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/"
list_of_runs_for_exclusion_dir  = "/work/halld/home/zhikun/lumi_skim/list_of_runs/exclusion_dir/"
list_of_files_to_exclude_dir = "/work/halld/home/zhikun/lumi_skim/list_of_files/exclusion_dir/files_to_exclude/"
list_of_files_after_exclusion_dir = "/work/halld/home/zhikun/lumi_skim/list_of_files/list_of_files_after_exclusion/"
list_of_runs_file_name_template = "list_of_exclusion_runs"
list_of_files_file_name_template = "list_Run{run_id}"
evio_files_name_template = "ps_{run_id}_{file_id}.evio"


bad_files_info_1 = {}
bad_files_info_2 = {}
bad_runs_and_files_only_id_shared = {}
bad_files_shared = {}




for he_version in he_versions:
    res_1_path = res_path_1_template.format(he_version=he_version,extraInfo=extraInfo)
    res_2_path = res_path_2_template.format(he_version=he_version,extraInfo=extraInfo)
    bad_files_info_1,  bad_files_info_2 = process_error_messages.check_all(res_1_path, res_2_path)
    # print(bad_files_info_1, bad_files_info_2)
    bad_files_shared = process_error_messages.compare_result(bad_files_info_1, bad_files_info_2)
    pprint(bad_files_shared)

for key in bad_files_shared:
    process_error_messages.add_bad_run(key[0], key[1], bad_runs_and_files_only_id_shared)


for key, value in bad_runs_and_files_only_id_shared.items():
    run_id = key
    list_of_files_file_to_read_path = os.path.join(list_of_runs_from_mss_dir, list_of_files_file_name_template)
    list_of_files_file_to_read_path = list_of_files_file_to_read_path.format(run_id=key)
    list_of_files_to_exclude_path = os.path.join(list_of_files_to_exclude_dir, list_of_files_file_name_template)
    list_of_files_to_exclude_path = list_of_files_to_exclude_path.format(run_id=key)
    list_of_files_file_after_excludion_path = os.path.join(list_of_files_after_exclusion_dir, list_of_files_file_name_template)
    list_of_files_file_after_excludion_path = list_of_files_file_after_excludion_path.format(run_id=key)
    with open(list_of_files_file_to_read_path, "r") as f1:
        lines = [line.strip() for line in f1.readlines()]
    with open(list_of_files_file_after_excludion_path, "w") as f_after_excludion:
        with open(list_of_files_to_exclude_path, "w") as f_to_exclude:
            for file_id in value:
                num_excluded_files += 1
                evio_file_name = evio_files_name_template.format(run_id=run_id, file_id=file_id)
                f_to_exclude.write(evio_file_name +"\n")
                lines.remove(evio_file_name)
                if delete_files:
                    files_to_delete_path = files_to_delete_template.format(run_id=run_id, file_id=file_id)
                    # Check if the file exists before attempting to delete
                    if os.path.exists(files_to_delete_path):
                        try:
                            # Use subprocess.run() with a list of arguments to avoid shell injection
                            subprocess.run(["rm", files_to_delete_path], check=True)
                            print(f"File {files_to_delete_path} deleted successfully.")
                        except subprocess.CalledProcessError as e:
                            print(f"Error deleting file {files_to_delete_path}: {e}")
                    else:
                        print(f"File {files_to_delete_path} does not exist.")
            for line in lines:
                f_after_excludion.write(line + "\n")
print(f"Total excluded files number = {num_excluded_files}")

with open("exclusion.log", "w") as f:
    for key, value in bad_runs_and_files_only_id_shared.items():
        f.write(key  + " " *4)
        for file_id in value:
            f.write(file_id + "  ")
        f.write("\n")