#include <avr/io.h>

int main(void) {
    PORTB = 0x00;
    DDRB |= (1 << PB0);

    PORTD |= (1 << PD2);
    DDRD = 0x00;

    while (1) {
        // PIND check l etat de toute les broches D
        // PIND & (1 << PD2) check filtre juste pour le bit qui correspond a PD2
        if (!(PIND & (1 << PD2))) {
            PORTB |= (1 << PB0);
        } else {
            PORTB = 0x00;
        }
    }

    return 0;
}