import numpy as np
import matplotlib.pyplot as plt

# ---------- Parâmetros ----------
fs = 10000  # Taxa de amostragem (Hz)

# Portadora
f_c = 500  # Frequência da portadora (100-999 Hz)
A_c = 1    # Amplitude da portadora

# ----- Mensagem analógica (AM e FM) -----
f_m = 5    # Frequência da mensagem (1-10 Hz)
A_m = 127.5  # Amplitude da mensagem (meio de 0 a 255)
offset = 127.5  # Para garantir que a senoide fique entre 0 e 255

# Tempo
t = np.arange(0,1, 1/fs)

# Sinal de mensagem analógica
m_analog = A_m * np.sin(2 * np.pi * f_m * t) + offset

# ---------- Modulação AM ----------
am = (A_c + (m_analog / 255)) * np.cos(2 * np.pi * f_c * t)

# ---------- Modulação FM ----------
kf = 50  # Sensibilidade de frequência
integral_m = np.cumsum((m_analog - 127.5) / 127.5) / fs  # Centraliza em zero
fm = A_c * np.cos(2 * np.pi * f_c * t + 2 * np.pi * kf * integral_m)

# ----- Mensagem digital (ASK e FSK) -----
bit_rate = 20  # bps (de 8 até 80 bps)
bits = [0, 1, 1, 0, 1, 1, 0, 0]  # Gera 8 bits aleatórios
print(f"Bits transmitidos: {bits}")

bit_duration = 1 / bit_rate
samples_per_bit = int(fs * bit_duration)

# Sinal digital expandido
bitstream = np.repeat(bits, samples_per_bit)
t_digital = np.arange(0, len(bitstream)) / fs

# ---------- Modulação ASK ----------
ask = A_c * bitstream * np.cos(2 * np.pi * f_c * t_digital)

# ---------- Modulação FSK ----------
f1 = f_c + 100  # Frequência para bit 1
f0 = f_c - 100  # Frequência para bit 0

fsk = A_c * np.cos(2 * np.pi * (f1 * (bitstream) + f0 * (1 - bitstream)) * t_digital)

# ---------- Plotagem ----------
plt.figure(figsize=(15, 10))

# AM
plt.subplot(4, 1, 1)
plt.title('Modulação AM')
plt.plot(t, am, color='b')
plt.xlabel('Tempo (s)')
plt.ylabel('Amplitude')
plt.grid()

# FM
plt.subplot(4, 1, 2)
plt.title('Modulação FM')
plt.plot(t, fm, color='r')
plt.xlabel('Tempo (s)')
plt.ylabel('Amplitude')
plt.grid()

# ASK
plt.subplot(4, 1, 3)
plt.title('Modulação ASK')
plt.plot(t_digital, ask, color='g')
plt.xlabel('Tempo (s)')
plt.ylabel('Amplitude')
plt.grid()

# FSK
plt.subplot(4, 1, 4)
plt.title('Modulação FSK')
plt.plot(t_digital, fsk, color='m')
plt.xlabel('Tempo (s)')
plt.ylabel('Amplitude')
plt.grid()

plt.tight_layout()
plt.show()
