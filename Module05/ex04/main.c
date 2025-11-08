#include "main.h"

typedef enum {
    RV1 = 0,
    LDR = (1 << MUX0),
    NTC = (1 << MUX1),
    TEMP_INTERNE = (1 << MUX3)
} adc_channel_t;

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

void led_value_convertor(uint8_t value) {
    uint8_t max = 255;

    PORTB = 0;
    if (value >= max * 0.25)
        PORTB |= (1 << PB0); // Bit0 → PB0
    if (value >= max * 0.5)
        PORTB |= (1 << PB1); // Bit1 → PB1
    if (value >= max * 0.75)
        PORTB |= (1 << PB2); // Bit2 → PB2
    if (value == max)
        PORTB |= (1 << PB4); // Bit3 → PB4
}

void init_rgb(void) {
    DDRD = (1 << PD3) | (1 << PD6) | (1 << PD5);

    TCCR0A |= (1 << WGM00) | (1 << WGM01); // Fast PWM mode
    TCCR0B |= (1 << CS02);                 // Prescaler = 256
    // clear on compare match, set at BOTTOM so = 1
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);

    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Fast PWM mode
    TCCR2A |= (1 << COM2B1);
    TCCR2B |= (1 << CS22) | (1 << CS21); // Prescaler = 256
}

uint8_t adc_read_channel(adc_channel_t channel) {
    //  5V
    ADMUX = (1 << REFS0) | (1 << ADLAR) | channel;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
    }
    return ADCH;
}

int main(void) {
    init_rgb();
    uart_init();
    uint8_t buffer_size = 10;
    char buffer[buffer_size];
    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

    // prescalar 128    16MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    uint8_t value = 0;
    while (1) {
        value = adc_read_channel(RV1);
        wheel(value);
        led_value_convertor(value);
        itoa(value, buffer, buffer_size);
        uart_printstr(buffer);
        uart_printstr("\r\n");
        _delay_ms(20);
    }

    return 0;
}
