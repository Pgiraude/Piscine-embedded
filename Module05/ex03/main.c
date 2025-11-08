#include "main.h"

// cf forum
float temp_sensor_offset = 324;
float coeff = 1.22;

typedef enum {
    RV1 = 0,
    LDR = (1 << MUX0),
    NTC = (1 << MUX1),
    TEMP_INTERNE = (1 << MUX3)
} adc_channel_t;

uint16_t adc_read_channel(adc_channel_t channel) {
    // internal 1.1V
    ADMUX = (1 << REFS1) | (1 << REFS0) | channel;
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

    uint16_t value_celsius = 0;
    while (1) {
        // T = { [(ADCH << 8) | ADCL] - TOS } / k
        value_celsius =
            (adc_read_channel(TEMP_INTERNE) - temp_sensor_offset) / coeff;
        itoa(value_celsius, buffer, buffer_size);
        uart_printstr(buffer);
        uart_printstr("\r\n");
        _delay_ms(20);
    }

    return 0;
}
