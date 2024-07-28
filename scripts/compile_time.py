import os
import time

"""Print the latest tutorial from Real Python"""
tic = time.perf_counter()
os.system('build.bat')
toc = time.perf_counter()
print(f"Compiling time is {toc - tic:0.4f} seconds")
