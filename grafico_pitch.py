import numpy as np
import matplotlib.pyplot as plt
arxiu1='rl001_wavesurfer.txt'
arxiu2='/Users/sergi24sanchez/PAV/P3/pitch_db/train/rl001.f0' 

wavesurfer = np.loadtxt(arxiu1,skiprows=0)
estimacion = np.loadtxt(arxiu2,skiprows=0)

dm = 0.015
#Time vector
samples = []
for i in range(0,len(estimacion)):
    samples.append(i*dm)


plt.plot(samples,estimacion,'g',lineWidth=1,label='Pitch detector')
plt.plot(samples,wavesurfer,'r',lineWidth=1.5,label='Wavesurfer pitch detector')
plt.title('Estimacio de Pitch')
plt.xlabel('Time[s]')
plt.ylabel('Frequency[Hz]')
plt.xlim(0,1.24)
plt.legend(loc='upper right')
plt.show()