#include <avr/io.h>

int main (void) {

    DDRB |= (1 << PB1);
    volatile unsigned long int i = 235294;
    while (1) {
        PORTB ^= (1 << PB1);
        while (i > 0) i--;
        i = 235294;
    }
    return 0;
}
// Assembly conversion of the program
// ~34 cycles per iteration of while loop (AI analysis)
// 16MHz clock source (cf datasheet)
// 16000000 / 34 ~= 470588 while loops for one second
// 470588 / 2 ~= 235294 for 0.5s