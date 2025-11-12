#include "main.h"

#define IS_AHT20_INIT 0x71
#define INIT_AHT20 0xBE
#define AHT20_ADRESSE 0x38
#define AHT20_INIT_MESURE 0xAC
#define WRITE 0
#define READ 1

uint8_t read_byte;

typedef enum {
    ERROR_1 = (1 << PB0),
    ERROR_2 = (1 << PB1),
    ERROR_3 = (1 << PB2),
    ERROR_4 = (1 << PB0) | (1 << PB1)
} t_error;

void ft_error(t_error error) {
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    PORTB |= (error & 0x07);

    TWCR = 0;
    while (1)
        ;
}

void print_hex_value(char c) {
    char buffer_size = 10;
    char buffer[buffer_size];
    convert_decimal_to_hex((uint8_t)c, buffer, buffer_size, 2);
    uart_printstr(buffer);
    uart_printstr(" ");
}

uint8_t i2c_status(void) {
    return (TWSR & TW_STATUS_MASK); // ONLY GET STATUS masking 3 first bits
                                    // (prescaler + reserve bits)
}

void i2c_write(unsigned char data) // send data  ? need to put adresse somewere
{
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
    }
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
    }
}

void i2c_read_ACK(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT))) {
    }
    if (i2c_status() != TW_MR_DATA_ACK)
        ft_error(ERROR_3);
    print_hex_value(TWDR);
}

void i2c_read_NACK(void) {
    TWCR = (1 << TWINT) | (1 << TWEN); // NACK
    while (!(TWCR & (1 << TWINT))) {
    }
    if (i2c_status() != TW_MR_DATA_NACK)
        ft_error(ERROR_4);
    print_hex_value((uint8_t)TWDR);
}

void i2c_read(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {
    }

    for (int i = 0; i < 7; i++) {
        if (i < 6) {
            i2c_read_ACK();
        } else {
            i2c_read_NACK();
        }
    }
}

void i2c_init(void) {
    // SCLfreq = 16Mkz / (16 + 2 * TWBR * prescaler)
    // pour SCLfreq = 100kHz <=> TWBR = 72
    // prescalar = 1 by default
    TWBR = 72;
}

void i2c_start(void) {
    TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {
    }
    if ((TWSR & TW_STATUS_MASK) != TW_START)
        ft_error(ERROR_1);
}

void i2c_stop(void) { TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); }

int main(void) {

    uart_init();
    i2c_init();
    _delay_ms(40);

    i2c_start();
    TWDR = (AHT20_ADRESSE << 1) | WRITE;
    i2c_write(INIT_AHT20);
    i2c_write(0x08);
    i2c_write(0x00); // pas d ACK ici
    i2c_stop();
    _delay_ms(40);

    while (1) {
        i2c_start(); // check if init done
        TWDR = (AHT20_ADRESSE << 1) | WRITE;
        i2c_write(0xAC);
        i2c_write(0x33);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(100); // if ok wait min wait 85ms

        i2c_start();                        // check if init done
        TWDR = (AHT20_ADRESSE << 1) | READ; // send adress and mode
        i2c_read();
        uart_printstr("\r\n");
        i2c_stop();
        _delay_ms(2000); // cf documentation
    }

    return 0;
}
