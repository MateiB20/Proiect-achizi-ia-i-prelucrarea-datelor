import numpy as np

from scipy.io import wavfile
samplerate, data = wavfile.read('sample_data/28.wav')
start_time = 0
duration = 6
start_sample = start_time * samplerate
end_sample = start_sample + duration * samplerate
segment = data[start_sample:end_sample]

wavFileInfo = open("waveInfo.txt", "a")
wavFileInfo.write(str(samplerate)+'\n')
wavFileInfo.write(str(segment.size)+'\n')
wavFileInfo.close()

print(samplerate)
print(segment.size)
print (segment)

np.savetxt("waveData.txt", segment, fmt="%2.0f")
