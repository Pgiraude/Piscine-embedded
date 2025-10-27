#include <avr/io.h>
#include <util/delay.h>

int led_value_convertor(int value) {

    PORTB = 0;

    if (value & (1 << 0)) PORTB |= (1 << PB0);  // Bit0 → PB0
    if (value & (1 << 1)) PORTB |= (1 << PB1);  // Bit1 → PB1
    if (value & (1 << 2)) PORTB |= (1 << PB2);  // Bit2 → PB2
    if (value & (1 << 3)) PORTB |= (1 << PB4);  // Bit3 → PB4

}

int main(void) {
    PORTB = 0x00;
    DDRB = 0xFF;  // Configue tous les PB

    PORTD |= (1 << PD2);
    PORTD |= (1 << PD4);
    DDRD = 0x00;

    int button_state_incr = 0;
    int past_button_state_incr = button_state_incr;
    int button_state_dcr = 0;
    int past_button_state_dcr = button_state_dcr;
    int value = 0;

    while (1) {
        past_button_state_incr = button_state_incr;
        button_state_incr = (PIND & (1 << PD2)) ? 1 : 0;
        if (past_button_state_incr == 1 && button_state_incr == 0) {
            _delay_ms(100);
            value++;
        }

        past_button_state_dcr = button_state_dcr;
        button_state_dcr = (PIND & (1 << PD4)) ? 1 : 0;
        if (past_button_state_dcr == 1 && button_state_dcr == 0) {
            _delay_ms(100);
            value--;
        }

        if (value < 0) {
            value = 0;
        } if (value > 15) {
            value = 15;
        }

        // Afficher la valeur en binaire sur les LED
        led_value_convertor(value);
    }
    return 0;
}