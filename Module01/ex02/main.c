#include <avr/io.h>
#include <util/delay.h>

#define PERIOD 15625
// PRESCALER divide microcontroller speed
// initial speed is 16MHz => 16000000 / 1024 = 15625 Hz

int main(void) {
    DDRB |= (1 << PB1);
    TCCR1B |= (1 << CS12) | (1 << CS10); // PRESCALER = 1 0 1 => 1024

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    // MODE 14 FAST PWM
    
    TCCR1A |= (1 << COM1A1);
    // cf doc => Clear OC1A/OC1B on Compare Match, set
    // OC1A/OC1B at BOTTOM (non-inverting mode)
    // BOTTOM signal = HIGH

    OCR1A = PERIOD * 0.9; // compare value then clear OC1A (high => low) if == OCR1A
    ICR1 = PERIOD; // ICR1 = TOP signal before reset cf mode 14 FAST PWM
    while (1) {}

    return 0;
}

