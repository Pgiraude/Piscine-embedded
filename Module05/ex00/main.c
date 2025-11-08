#include "main.h"

uint8_t adc_read(void) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
    }
    return ADCH;
}

int main(void) {
    uart_init();
    uint8_t buffer_size = 10;
    char buffer[buffer_size];
    ADMUX = (1 << REFS0);
    ADMUX |= (1 << ADLAR);
    // prescalar 128    16MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    while (1) {
        convert_decimal_to_hex(adc_read(), buffer, buffer_size, 2);
        uart_printstr(buffer);
        uart_printstr("\r\n");
        _delay_ms(20);
    }

    return 0;
}
