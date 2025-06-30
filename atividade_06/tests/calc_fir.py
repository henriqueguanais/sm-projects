import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import firwin, lfilter, freqz

# Parâmetros
fs = 4000          # Frequência de amostragem (Hz)
cutoff = 5      # Frequência de corte (Hz)
numtaps = 16       # Número de coeficientes (ordem + 1)

# Criação do filtro FIR com janela de Hamming
h = firwin(numtaps, cutoff, fs=fs, window='hamming') 
print(h)
# Visualizar resposta em frequência
w, H = freqz(h, worN=8000, fs=fs)
plt.plot(w, 20 * np.log10(np.abs(H)))
plt.title('Resposta em Frequência do Filtro FIR')
plt.xlabel('Frequência (Hz)')
plt.ylabel('Magnitude (dB)')
plt.grid()
plt.show()

# Aplicando o filtro a um sinal
t = np.linspace(0, 1.0, fs)
x = np.sin(2*np.pi*2*t) + 0.5*np.sin(2*np.pi*400*t)  # Sinal com 

y = lfilter(h, 1.0, x)  # Aplicação do filtro

# Plot
plt.plot(t, x, label='Original')
plt.plot(t, y, label='Filtrado')
plt.legend()
plt.title('Sinal Original vs Filtrado')
plt.xlabel('Tempo (s)')
plt.show()