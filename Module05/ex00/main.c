#include "main.h"

uint8_t adc_read(void) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
    }
    return ADCH;
}

int main(void) {
    uart_init();
    char hex_str[10];
    ADMUX = (1 << REFS0);
    ADMUX |= (1 << ADLAR);
    // prescalar 128    16MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    while (1) {
        convert_decimal_to_hex(adc_read(), hex_str);
        uart_printstr(hex_str);
        uart_printstr("\r\n");
        _delay_ms(20);
    }

    return 0;
}
