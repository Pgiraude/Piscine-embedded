#include "main.h"

#define INIT_AHT20 0xBE
#define AHT20_ADRESSE 0x38
#define WRITE 0
#define READ 1

#include <util/delay.h>

typedef enum {
    ERROR_1 = (1 << PB0),
    ERROR_2 = (1 << PB1),
    ERROR_3 = (1 << PB2)
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

uint8_t i2c_status(void) {
    return (TWSR & TW_STATUS_MASK); // ONLY GET STATUS masking 3 first bits
                                    // (prescaler + reserve bits)
}

void i2c_write(uint8_t data) // send data  ? need to put adresse somewere
{
    TWDR = (AHT20_ADRESSE << 1) | WRITE; // send adress and mode

    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
    }

    if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK)
        ft_error(ERROR_2);

    TWDR = data;

    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
    }

    // if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
    //     ft_error(ERROR_3);
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

    i2c_start();
    i2c_write(INIT_AHT20);
    uart_tx(i2c_status());
    uart_printstr("\r\n");
    i2c_stop();
    _delay_ms(40);

    i2c_start(); // check if init done
    i2c_write(0x71);
    uart_tx(i2c_status());
    uart_printstr("\r\n");
    i2c_stop();
    _delay_ms(10); // if ok wait 10ms

    while (1) {
        i2c_start();
        i2c_write('A');
        uart_tx(i2c_status());
        uart_printstr("\r\n");
        i2c_stop();
        _delay_ms(50);
    }

    return 0;
}
