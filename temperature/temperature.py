import matplotlib.pyplot as plt
import datetime
import matplotlib.dates as mdates

# 原有的读取并绘图函数
files = [f"temperature_log_temp{i}.log" for i in [1, 2, 3, 4]]
# files = [f"temperature_log_temp{i}.log" for i in [1]]

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
        # 拼接日期和时间
        time_str = f"{parts[0]} {parts[1]}"
        value = float(parts[2])
        times.append(datetime.datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S"))
        values.append(value)

    # 绘图
    plt.figure(figsize=(120, 5))
    plt.plot(times, values, marker='o', linestyle='-', color='b', markersize=0.5)
    plt.xlabel("Time")
    plt.ylabel("Value")
    plt.title(f"Time Series Data: {fileName}")
    plt.gca().xaxis.set_major_locator(mdates.DayLocator(interval=1))
    plt.gca().xaxis.set_minor_locator(mdates.HourLocator(interval=6))
    plt.grid(which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.xlim(times[0], times[-1])
    plt.ylim(5, 45)
    plt.savefig(fileName.replace(".log", ".pdf"), format="pdf", dpi=300, bbox_inches='tight')
    plt.show()

# 新增的辅助函数，用于挑选指定时间段内的数据
def get_temperature_data_in_range(fileName, start_time, end_time):
    """
    从指定的日志文件中读取温度数据，并挑选出在 [start_time, end_time] 范围内的数据

    参数：
        fileName: 日志文件名
        start_time: 开始时间，datetime 对象
        end_time: 结束时间，datetime 对象

    返回：
        filtered_times: 在指定时间范围内的时间列表
        filtered_values: 在指定时间范围内对应的温度值列表
    """
    with open(fileName, 'r') as f:
        lines = f.readlines()

    filtered_times = []
    filtered_values = []
    for line in lines:
        line = line.strip()
        parts = line.split()
        if len(parts) != 3:
            continue
        # 拼接日期和时间
        time_str = f"{parts[0]} {parts[1]}"
        dt = datetime.datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S")
        # 判断时间是否在指定区间内
        if start_time <= dt <= end_time:
            filtered_times.append(dt)
            filtered_values.append(float(parts[2]))
    return filtered_times, filtered_values

# 示例：如何使用 get_temperature_data_in_range 函数
if __name__ == "__main__":
    # 假设我们需要挑选出2025-01-01 00:00:00 到 2025-01-02 00:00:00 之间的数据
    start = datetime.datetime.strptime("2025-01-16 12:00:00", "%Y-%m-%d %H:%M:%S")
    end   = datetime.datetime.strptime("2025-01-23 00:00:00", "%Y-%m-%d %H:%M:%S")
    for file_to_filter in files:
    # 例如，对第一个文件进行数据过滤并绘制  
        file = file_to_filter
        filtered_times, filtered_values = get_temperature_data_in_range(file_to_filter, start, end)
        
        # 绘制过滤后的数据
        plt.figure(figsize=(20, 5))
        plt.plot(filtered_times, filtered_values, marker='o', linestyle='-', color='r', markersize=0.5)
        plt.xlabel("Time")
        plt.ylabel("Value")
        plt.title(f"Filtered Data from {file_to_filter}\n({start} to {end})")
        ax = plt.gca()

        # 主要刻度：每1天显示日期
        # ax.xaxis.set_major_locator(mdates.DayLocator(interval=2))
        # ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))

        # 次要刻度：每6小时显示时间，但避开午夜（0:00）
        ax.xaxis.set_major_locator(mdates.HourLocator(byhour=[0, 6, 12, 18]))
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))

        # 调整主刻度标签的位置
        ax.xaxis.set_tick_params(which='major', pad=10)  # pad 参数控制刻度标签与轴的距离
        # 设置X轴标题并往下偏移
        ax.set_xlabel('Time', labelpad=20)  # 增加labelpad的值来往下偏移X轴标题
        current_date = None
        for date in filtered_times:
            if date.hour == 0 and (current_date is None or date.date() != current_date):
                current_date = date.date()
                ax.text(date, -0.1, date.strftime('%Y-%m-%d'), ha='center', va='top', fontsize=10, transform=ax.get_xaxis_transform())
        # 确保次刻度标签不会被主刻度标签覆盖
        # ax.xaxis.set_tick_params(which='minor', pad=10)  # 调整次刻度标签与轴的距离
        # # 次要刻度：每天中午12点显示日期
        # ax.xaxis.set_minor_locator(mdates.HourLocator(byhour=[12]))  # 每天中午12点生成刻度
        # ax.xaxis.set_minor_formatter(mdates.DateFormatter('\n%Y-%m-%d'))

        # 强制显示次要刻度标签并居中
        # plt.setp(ax.get_xminorticklabels(), rotation=0, ha='center', va='top')
        plt.grid(which='both', linestyle='--', linewidth=0.5)
        plt.tight_layout()
        plt.savefig(f"{file}_from_{start}_to_{end}.pdf".replace(":","."))
        plt.show()

    # 依次绘制所有文件的完整数据
        plot_figure(file)
