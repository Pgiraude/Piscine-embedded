#include "main.h"

uint8_t count = 0;

void led_value_convertor(uint8_t value) {

    PORTB = 0;
    if (value & (1 << 0))
        PORTB |= (1 << PB0); // Bit0 → PB0
    if (value & (1 << 1))
        PORTB |= (1 << PB1); // Bit1 → PB1
    if (value & (1 << 2))
        PORTB |= (1 << PB2); // Bit2 → PB2
    if (value & (1 << 3))
        PORTB |= (1 << PB4); // Bit3 → PB4
}

ISR_PIN_CHANGE_2 {
    static uint8_t lst_portd_state = 0xFF;
    // Check electric states => pull-up = 1 et pull-down = 0
    uint8_t cur_portd_state = PIND;

    // Detection off changes
    uint8_t changed = cur_portd_state ^ lst_portd_state;
    // PD2 changes increment only on pull-down
    if ((changed & (1 << PD2)) && !(cur_portd_state & (1 << PD2))) {
        count++;
        led_value_convertor(count);
        while (!(PIND & (1 << PD2))) {
            _delay_ms(1);
        }
        PCIFR |= (1 << PCIF2);
    }
    // PD4 changes decrement only on pull-down
    if ((changed & (1 << PD4)) && !(cur_portd_state & (1 << PD4))) {
        count--;
        led_value_convertor(count);
        _delay_ms(5);
        PCIFR |= (1 << PCIF2);
    }
    lst_portd_state = cur_portd_state;
    // PCIFR |= (1 << PCIF2);
}

int main(void) {

    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT20); // Pin PD2 PD4
    sei();

    while (1) {
    }

    return 0;
}