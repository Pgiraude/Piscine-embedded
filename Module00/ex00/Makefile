MCU = atmega328p
F_CPU = 16000000UL # 16MHz clock source cf devboard microcontroller

NAME = main
SRC = $(NAME).c

BAUD_RATE = 115200 # cf subject vitesse de communication serie (arduino)

PORT = /dev/ttyUSB0 # dans les fichiers other port USB sous linux

BOARD = arduino # bootloader (permet de lancer un systeme d exploitation arduino)

CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os # -Os pour optimiser le code embarque

all: hex flash

$(NAME).bin: $(NAME).c
	$(CC) $(CFLAGS) $(SRC) -o $(NAME).bin 

hex: $(NAME).bin # Convert le fichier .bin (.elf) en format Intel HEX (.hex)
	avr-objcopy -O ihex $(NAME).bin $(NAME).hex
# oblige pour microcontroleur
# avr-objcopy : converteur de fichiers  
# -O ihex : format de sortie Intel HEX pour microcontroleur (compatible avrdude)
# avr-objcopy --info

flash: $(NAME).hex
	avrdude -p $(MCU) -c $(BOARD) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(NAME).hex
# cf man 
# programme le firmware sur le microcontroleur via le port serie et l utilitaire avrdude
# -U flash:w:$(NAME).hex : operation d upload dans la memoire flash
# -U : use (utilise les spécifications suivantes)
# flash : type de mémoire (flash du microcontroleur)
# w: : write

clean:
	rm -f $(NAME).bin $(NAME).hex

.PHONY: all hex flash clean
