#include "main.h"

ISR_PIN_CHANGE_2 {

    static uint8_t is_pressed = 0;
    is_pressed = (is_pressed + 1) % 2;

    if (is_pressed) {
        PORTB ^= (1 << PB0);
        _delay_ms(5);
    }
    // PCIFR => Pin change interrupt flag register
    PCIFR |= (1 << PCIF2); // clear interrupt flag
}

int main(void) {
    DDRB |= (1 << PB0);
    // PCICR => Pin change interrupt control register
    PCICR |= (1 << PCIE2); // set interrupt
    // PCMSK2 => Pin change mask register
    PCMSK2 |= (1 << PCINT18); // target pin PD2
    sei();

    while (1) {
    }

    return 0;
}
