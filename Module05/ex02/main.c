#include "main.h"

typedef enum {
    RV1 = 0,
    LDR = 1, // MUX0
    NTC = 2  // MUX1
} adc_channel_t;

uint16_t adc_read_channel(adc_channel_t channel) {
    ADMUX = (1 << REFS0) | channel;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
    }
    uint16_t value = ADCL | (ADCH << 8);
    return value;
}

int main(void) {
    uart_init();
    uint8_t buffer_size = 10;
    char buffer[buffer_size];

    // prescalar 128    16MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    while (1) {
        itoa(adc_read_channel(RV1), buffer, buffer_size);
        uart_printstr(buffer);
        uart_printstr(", ");
        itoa(adc_read_channel(LDR), buffer, buffer_size);
        uart_printstr(buffer);
        uart_printstr(", ");
        itoa(adc_read_channel(NTC), buffer, buffer_size);
        uart_printstr(buffer);
        uart_printstr("\r\n");
        _delay_ms(20);
    }

    return 0;
}
