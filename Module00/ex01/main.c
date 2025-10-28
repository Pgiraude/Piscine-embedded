#include <avr/io.h>

int main(void) {
    DDRB |= (1 << PB0); // set le bit PB0 de DDRB => memoire adresse0x2A a 1
    PORTB = (1 << PB0); // set le bit PB0 de PORTB => 0x25 a 1
    // unsigned char *portb_ptr = (unsigned char *)0x25;
    // *portb_ptr = (1 << PB1);  // Écrit directement à l'adresse 0x25 de PORTB
    // page 100
    return 0;
}