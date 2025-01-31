import asyncio
import os
import time

list_of_runs_file = "test.txt"  # 运行列表文件
source_dir = r"/volatile/halld/home/zhikun/merged_new_runs/"  # 源目录
dest_dir = r"/mss/halld/RunPeriod-2022-08/production/all_lumi_new/"  # 目标目录
pausetime = 10  # 状态显示的更新间隔（秒）
max_concurrent_tasks = 6  # 最大并发任务数

# 读取运行列表文件
with open(list_of_runs_file, "r") as f:
    runs = [line.strip() for line in f if line.strip()]

async def mirror_run(run, status_dict, start_times, semaphore):
    """复制单个运行并监控进度"""
    source_path = os.path.join(source_dir, f"Run{run}")
    dest_path = os.path.join(dest_dir, f"Run{run}")

    async with semaphore:  # 控制并发数量
        try:
            # 记录开始时间
            start_times[run] = time.time()
            status_dict[run] = "Running"
            with open("log_mss.log", 'a') as f:
                process = await asyncio.create_subprocess_exec(
                    "jmirror", source_path, source_path, dest_path,
                    stdout=f,  # 重定向 stdout 到文件
                    stderr=f,  # 重定向 stderr 到文件
                )
                await process.wait()

                # 检查复制进度
                while True:
                    if not os.path.exists(dest_path) or set(os.listdir(source_path)) != set(os.listdir(dest_path)):
                        await asyncio.sleep(pausetime)  # 确保检查不会过于频繁
                    else:
                        print(f"\nRuns {run} finished")
                        break

                # 更新状态
                status_dict[run] = "Completed"
        except Exception as e:
            status_dict[run] = f"Error: {e}"

# async def display_status(status_dict, start_times):
#     """每10秒更新一次状态，并显示用时"""
#     while True:
#         current_time = time.time()
#         status_line = " | ".join(
#             f"Run {run}: {status} ({current_time - start_times.get(run, current_time):.1f}s)"
#             for run, status in status_dict.items()
#         )
#         print(f"\r{status_line}", end="", flush=True)
#         await asyncio.sleep(pausetime)

#         # 检查是否所有任务都已完成
#         if all(status in ("Completed", "Error") for status in status_dict.values()):
#             break
#     print()  # 换行以结束显示


async def display_status(status_dict, start_times, pausetime=10):
    """每10秒更新一次状态，并显示用时，每行显示5个run的状态"""
    while True:
        current_time = time.time()
        status_lines = []
        for i in range(0, len(status_dict), 5):
            batch = list(status_dict.items())[i:i+5]
            status_line = " | ".join(
                f"Run {run}: {status} ({current_time - start_times.get(run, current_time):.1f}s)"
                for run, status in batch
            )
            status_lines.append(status_line)
        
        # 计算当前的行数
        num_lines = len(status_lines)
        
        # 先回到前面打印的部分，清除所有行
        print("\r", end="")  # 回到行首
        for _ in range(num_lines):
            print("\033[2K\033[F", end="")  # 清除行并上移
        
        # 重新打印新的状态
        print("\n".join(status_lines), flush=True)

        await asyncio.sleep(pausetime)

        # 检查是否所有任务都已完成
        if all(status in ("Completed", "Error") for status in status_dict.values()):
            break
    print()  # 换行以结束显示

async def main():
    """主任务，处理所有运行"""
    status_dict = {run: "Pending" for run in runs}  # 初始化任务状态
    start_times = {}  # 记录每个任务的开始时间
    semaphore = asyncio.Semaphore(max_concurrent_tasks)  # 创建信号量

    # 启动复制任务
    tasks = [mirror_run(run, status_dict, start_times, semaphore) for run in runs]

    # 启动状态显示任务
    display_task = asyncio.create_task(display_status(status_dict, start_times))

    # 等待所有任务完成
    await asyncio.gather(*tasks)
    await display_task

# 启动异步任务
asyncio.run(main())