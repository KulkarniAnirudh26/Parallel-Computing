import numpy as np 
import matplotlib.pyplot as plt


Time = [1943.48,374.357, 197.325, 143.43, 261.711];
#Performance = [70.7177,146.722 ,258.259, 295.151]
#labels = ['naive', 'block size 4', 'block size 8', 'block size 16']
index = np.arange(len(Time))


plt.plot(index, Time,'ro-')
plt.xlabel('Different Number of threads', fontsize=10)
plt.ylabel('Execution Time (in sec)', fontsize=10)
#plt.xticks(index, labels ,fontsize=10, rotation=0)
plt.title('Comparition of Execution times')
plt.show()

'''
plt.bar(index, Performance)
plt.xlabel('Different matrix multiplication methods', fontsize=10)
plt.ylabel('Performance (in MFLOPs/sec)', fontsize=10)
plt.xticks(index, labels ,fontsize=10, rotation=0)
plt.title('Comparition of Execution times')
plt.show()
'''