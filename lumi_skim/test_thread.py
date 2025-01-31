import threading
import time

def task1():
    for i in range(5):
        print(f"Task 1 - Iteration {i}")
        time.sleep(2)

def task2():
    for i in range(5):
        print(f"Task 2 - Iteration {i}")
        time.sleep(3)

# 创建线程
thread1 = threading.Thread(target=task1)
thread2 = threading.Thread(target=task2)

# 启动线程
thread1.start()
thread2.start()

# 等待线程完成
thread1.join()
thread2.join()

print("Both threads have finished.")
