he_versions = list(range(3, 11))

for he_version in he_versions:
    list_file_path1 = f"/work/halld3/home/somov/lumi_skim/list_of_runs_primex3/list_of_runs_he{he_version}"
    list_file_path2 = f"/work/halld/home/zhikun/lumi_skim/list_of_runs_primex3/list_of_runs_he{he_version}"
    
    with open(list_file_path1, 'r') as file1:
        with open(list_file_path2, 'r') as file2:
            lines1 = file1.readlines()
            lines2 = file2.readlines()
            
            # 使用集合去重并合并
            combined_lines = sorted(list(set(lines1 + lines2)))
            print(len(lines1),len(lines2),len(combined_lines))
            
            # 如果你需要保持原有顺序，可以使用以下方法
            # combined_lines = list(dict.fromkeys(lines1 + lines2))

            # 处理 combined_lines，例如写入新的文件
            output_file_path = f"/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/list_of_runs_he{he_version}"
            with open(output_file_path, 'w') as output_file:
                output_file.writelines(combined_lines)

print("Successfully merged")
