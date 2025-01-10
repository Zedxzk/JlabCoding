import os
from pprint import pprint
import process_error_messages
from findList import list_of_runs_dir, list_of_files_dir, evio_name_template


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
                        