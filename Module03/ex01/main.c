#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRD = (1 << PD3) | (1 << PD6) | (1 << PD5);

    while (1) {
        PORTD = (1 << PD5);
        _delay_ms(1000);
        PORTD = (1 << PD6);
        _delay_ms(1000);
        PORTD = (1 << PD3);
        _delay_ms(1000);
        PORTD = (1 << PD5) | (1 << PD6);
        _delay_ms(1000);
        PORTD = (1 << PD6) | (1 << PD3);
        _delay_ms(1000);
        PORTD = (1 << PD5) | (1 << PD3);
        _delay_ms(1000);
        PORTD = (1 << PD5) | (1 << PD6) | (1 << PD3);
        _delay_ms(1000);
    }

    return 0;
}
