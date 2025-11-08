#include "main.h"

typedef enum {
    RV1 = 0,
    LDR = 1,
    NTC = 2
} adc_channel_t;



uint8_t adc_read_channel(adc_channel_t channel) {
    ADMUX = (1 << REFS0) | (1 << ADLAR) | channel;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {}
    return ADCH;
}


int main(void) {
    uart_init();
    uint8_t buffer_size = 10;
    char buffer[buffer_size];


    // prescalar 128    16MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    while (1) {
        convert_decimal_to_hex(adc_read_channel(RV1), buffer, buffer_size, 2);
        uart_printstr(buffer);
        uart_printstr(", ");
        convert_decimal_to_hex(adc_read_channel(LDR), buffer, buffer_size, 2);
        uart_printstr(buffer);
        uart_printstr(", ");
        convert_decimal_to_hex(adc_read_channel(NTC), buffer, buffer_size, 2);
        uart_printstr(buffer);
        uart_printstr("\r\n");
        _delay_ms(20);
    }

    return 0;
}
