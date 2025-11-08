#include "main.h"

typedef enum { ERROR_1 = (1 << PB0) } t_error;

void ft_error(t_error error) {
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    PORTB |= (error & 0x07);

    TWCR = 0;
    while (1)
        ;
}

// SCLfreq = 16Mkz / (16 + 2 * TWBR * prescaler)
// pour SCLfreq = 100kHz <=> TWBR = 72
void i2c_init(void) { TWBR = 72; }

void i2c_start(void) {
    TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {
    }
    if ((TWSR & TW_STATUS_MASK) != TW_START)
        ft_error(ERROR_1);
}

void i2c_stop(void) { TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); }

int main(void) {
    i2c_init();

    while (1) {
        i2c_start();
        i2c_stop();
    }

    return 0;
}
