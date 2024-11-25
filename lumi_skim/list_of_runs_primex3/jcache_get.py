import subprocess
import logging

# 配置成功日志
success_log = 'success.log'
logging.basicConfig(
    filename=success_log,
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)

# 配置错误日志，使用不同的 logger
error_log = 'error.log'
error_logger = logging.getLogger('error')
error_handler = logging.FileHandler(error_log)
error_formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
error_handler.setFormatter(error_formatter)
error_logger.addHandler(error_handler)
error_logger.setLevel(logging.ERROR)

def fetch_files(run_id):
    mss_dir = f"/mss/halld/RunPeriod-2022-08/calib/ver01/PS/Run{run_id}/"
    logging.info(f"Fetching files from: {mss_dir}")
    
    # 拼接命令，使用 shell=True 来执行
    command = f"jcache get {mss_dir}*"
    try:
        result = subprocess.run(command, shell=True, check=True, capture_output=True, text=True)
        success_message = f"Fetch command executed successfully for Run {run_id}."
        
        if "status: failed" in result.stdout or "status: failed" in result.stderr:
            error_message = f"Fetch command failed for Run {run_id}: {result.stdout.strip()} {result.stderr.strip()}"
            print(error_message)
            error_logger.error(error_message)
        else:
            print(success_message)
            logging.info(success_message)

    except subprocess.CalledProcessError as e:
        error_message = f"Error occurred while fetching for Run {run_id}: {e}"
        print(error_message)
        error_logger.error(error_message)

def main():
    try:
        with open("list_of_runs_he4") as file:
            lines = file.readlines()
            for run_id in lines:
                run_id = run_id.strip()
                if run_id:  # 确保不处理空行
                    fetch_files(run_id)
        
    except FileNotFoundError as e:
        error_logger.error(f"File not found: {e}")
        print(f"File not found: {e}")
        exit(1)
    except Exception as e:
        error_logger.error(f"An error occurred: {e}")
        print(f"An error occurred: {e}")
        exit(1)

if __name__ == "__main__":
    main()