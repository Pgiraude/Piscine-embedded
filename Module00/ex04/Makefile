MCU = atmega328p
F_CPU = 16000000UL # default 8MHz clock source 

NAME = main
SRC = $(NAME).c

BAUD_RATE = 115200 # cf subject 

PORT = /dev/ttyUSB0

BOARD = arduino

CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os

all: hex flash

$(NAME).bin: $(NAME).c
	$(CC) $(CFLAGS) $(SRC) -o $(NAME).bin 

hex: $(NAME).bin
	avr-objcopy -O ihex $(NAME).bin $(NAME).hex


flash: $(NAME).hex
	avrdude -p $(MCU) -c $(BOARD) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(NAME).hex

clean:
	rm -f $(NAME).bin $(NAME).hex

.PHONY: all hex flash clean
