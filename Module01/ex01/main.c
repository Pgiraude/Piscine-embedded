#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << PB1);
    TCCR1B |= (1 << CS12) | (1 << CS10); // PRESCALER = 1 0 1 => 1024
    // PRESCALER divide microcontroller speed
    // initial speed is 16MHz => 16000000 / 1024 = 15625 Hz
    // soit 15625/2 = 7812 nb of ticks for 0.5s
    TCCR1B |= (1 << WGM12); // MODE CTC TOP => clear timer on compare match (OCR1A)

    TCCR1A |= (1 << COM1A0); // Toggle on compare match defined by OCR1A
    OCR1A = 15625 * 0.5; // nb of ticks for 0.5s
    while (1) {}

    return 0;
}

