import subprocess
import matplotlib.pyplot as plt

ticks = list(range(1, 100000, 1000))
results_default = []
results_custom = []

for i in ticks:
    output_default = int(subprocess.Popen(["..\\cmake-build-debug\\bin\\DefaultNAlloc.exe", "50", str(i)],
                                          stdout=subprocess.PIPE).communicate()[0].decode('utf-8'))
    output_custom = int(subprocess.Popen(["..\\cmake-build-debug\\bin\\MyNAlloc.exe", "50", str(i)],
                                         stdout=subprocess.PIPE).communicate()[0].decode('utf-8'))
    results_default.append(output_default)
    results_custom.append(output_custom)

plt.plot(ticks, results_default, label="Default allocator")
plt.plot(ticks, results_custom, label="Pool allocator")
plt.xlabel("Generations")
plt.ylabel("Time, ms")
plt.title("Allocation of 50 ints with N generations")
plt.legend()
plt.show()

ticks = list(range(1, 100000, 1000))
results_default = []
results_custom = []

for i in ticks:
    output_default = int(subprocess.Popen(["..\\cmake-build-debug\\bin\\DefaultNAlloc.exe", "500", str(i)],
                                          stdout=subprocess.PIPE).communicate()[0].decode('utf-8'))
    output_custom = int(subprocess.Popen(["..\\cmake-build-debug\\bin\\MyNAlloc.exe", "500", str(i)],
                                         stdout=subprocess.PIPE).communicate()[0].decode('utf-8'))
    results_default.append(output_default)
    results_custom.append(output_custom)

plt.plot(ticks, results_default, label="Default allocator")
plt.plot(ticks, results_custom, label="Pool allocator")
plt.xlabel("Generations")
plt.ylabel("Time, ms")
plt.title("Allocation of 500 ints with N generations")
plt.legend()
plt.show()
