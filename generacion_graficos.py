import numpy as np
import soundfile as sf
import matplotlib.pyplot as plt
import scipy.signal as sig
arxiu='/Users/sergi24sanchez/PAV/P3/pitch_db/train/rl001.wav'
x,fm=sf.read(arxiu)

#Time vector
samples = []
for i in range(0,len(x)):
    samples.append(i/fm)

#Autocorrelation
autocorrelation=np.correlate(x,x,"same")
m = np.arange(len(autocorrelation))
peaks = sig.find_peaks(autocorrelation)
[a1,a2]=np.split(autocorrelation,2)

#Plot generation
plt.subplot(2,1,1)
plt.plot(samples,x,lineWidth=0.5)
plt.title('30 ms of voiced frames')
plt.grid(True)
plt.xlabel('Time[s]')
plt.ylabel('Waveform')
plt.xlim(0.09,0.12)
plt.subplot(2,1,2)
plt.plot(a2,lineWidth=0.5)
plt.xlabel('Samples')
plt.ylabel('Autocorrelation')
plt.xlim(0,200)
plt.grid(True)
plt.show()
