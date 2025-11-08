#include "main.h"

int main(void) {

    uart_init();
    ADMUX = (1 << REFS0);

    while (1) {
        uart_printstr();
        _delay_ms(20);
    }

    return 0;
}