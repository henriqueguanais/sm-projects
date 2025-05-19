# Makefile para projeto AVR ATmega328P

# Configurações
MCU     = atmega328p
F_CPU   = 16000000UL
PORT    = /dev/ttyACM0
BAUD    = 115200
PROGRAMMER = arduino

# Diretórios
BUILD_DIR = build
SRC_DIR = src

# Arquivos fonte
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# Compilador e flags
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -Wall -Os -DF_CPU=$(F_CPU)
LDFLAGS = -mmcu=$(MCU)

.PHONY: all flash clean

all: $(BUILD_DIR)/firmware.hex

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)  # Cria o diretório se não existir
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/firmware.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/firmware.hex: $(BUILD_DIR)/firmware.elf
	avr-objcopy -O ihex -R .eeprom $< $@

flash: $(BUILD_DIR)/firmware.hex
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$<:i

clean:
	rm -rf $(BUILD_DIR)