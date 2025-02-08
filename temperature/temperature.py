import matplotlib.pyplot as plt
import datetime

files = [f"temperature_log_temp{i}.log" for i in [1, 2, 3, 4]]

def plot_figure(fileName):
# 解析数据
    with open(fileName, 'r') as f:
        lines = f.readlines()
    times = []
    values = []

    for line in lines:
        line = line.strip()
        parts = line.split()
        if len(parts) != 3:
            continue
        # print(parts)
        time_str = f"{parts[0]} {parts[1]}"
        value = float(parts[2])
        times.append(datetime.datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S"))
        values.append(value)

    # print(times)
    # 绘图
    plt.figure(figsize=(120, 5))
    plt.plot(times, values, marker='o', linestyle='-', color='b',markersize=0.5)
    plt.xlabel("Time")
    plt.ylabel("Value")
    plt.title(f"Time Series Data: {fileName}")
    # plt.xticks(rotation=45)
    plt.grid()
    plt.tight_layout()
    plt.ylim(5, 45)
    # plt.savefig(fileName.replace(".log",".jpg"),dpi=450)
    plt.savefig(fileName.replace(".log", ".pdf"), format="pdf", dpi=300, bbox_inches='tight')
    plt.show()

for file in files:
    plot_figure(file)
