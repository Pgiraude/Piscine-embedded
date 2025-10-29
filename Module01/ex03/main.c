#include <avr/io.h>
#include <util/delay.h>

#define PULL_UP 1
#define PULL_DOWN 0
#define DEBOUNCE_TIME 80
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

     // compare value then clear OC1A (high => low) if == OCR1A
    ICR1 = PERIOD; // ICR1 = TOP signal before reset cf mode 14 FAST PWM


    PORTD |= (1 << PD2) | (1 << PD4);
    DDRD = 0x00;
    // Buttons PD2 and PD4 configure as input;
    
    float percentage = 0.1;
    int btn_incr, btn_dcr = 0;
    int prev_btn_incr, prev_btn_dcr = 0;

    while (1) {

        OCR1A = PERIOD * percentage;

        prev_btn_incr = btn_incr;
        btn_incr = (PIND & (1 << PD2)) ? PULL_UP : PULL_DOWN;
        if (prev_btn_incr == PULL_UP && btn_incr == PULL_DOWN) {
            percentage += 0.1;
            if (percentage > 1) {
                percentage = 1;
            }
        }

        prev_btn_dcr = btn_dcr;
        btn_dcr = (PIND & (1 << PD4)) ? PULL_UP : PULL_DOWN;
        if (prev_btn_dcr == PULL_UP && btn_dcr == PULL_DOWN) {
            percentage -= 0.1;
            if (percentage < 0.1) {
                percentage = 0.1;
            }
        }
        _delay_ms(DEBOUNCE_TIME);
    }

    return 0;
}

