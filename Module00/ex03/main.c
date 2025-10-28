#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    PORTB = 0x00;
    DDRB |= (1 << PB0);

    PORTD |= (1 << PD2);
    DDRD = 0x00;

    int button_state = 0;
    int past_button_state = button_state;
    int led_state = 0;
    int pull_up = 1;
    int pull_down = 0;

    while (1) {
        past_button_state = button_state;
        button_state = (PIND & (1 << PD2)) ? pull_up : pull_down;
        if (past_button_state == pull_up && button_state == pull_down) {
            // on passe de l etat relache a l etat appuye
            led_state = !led_state;
        }

        if (led_state) {
            PORTB |= (1 << PB0);
        } else {
            PORTB = 0x00;
        }
        _delay_ms(80);
    }
    return 0;
}