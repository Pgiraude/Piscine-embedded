#include "main.h"

const char hex_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}

int8_t get_hex_index(char c) {
    uint8_t index = 0;
    while (hex_chars[index]) {
        if (hex_chars[index] == c) {
            return index;
        }
        index++;
    }
    return -1;
}

int16_t convert_hex_to_decimal(char *input, uint8_t end) {
    const uint8_t len = ft_strlen(input);
    if (end > len || len == 0) {
        return -1;
    }
    const uint8_t cycles = end ? end : len;

    int16_t value = 0;
    uint8_t index = 0;
    while (index < cycles) {
        if (get_hex_index(input[index]) == -1) {
            return -1;
        }
        value = value * 16 + get_hex_index(input[index]);
        index++;
    }
    return value;
}

void convert_hex_to_rgb(char *input) {
    int16_t r = 0;
    int16_t g = 0;
    int16_t b = 0;

    if (input[0] != '#' || ft_strlen(input) != 7) {
        uart_printstr("Invalid color format\r\n");
        return;
    }
    r = convert_hex_to_decimal(input + 1, 2);
    g = convert_hex_to_decimal(input + 3, 2);
    b = convert_hex_to_decimal(input + 5, 2);
    if (r == -1 || g == -1 || b == -1) {
        uart_printstr("Invalid color format\r\n");
        return;
    }
    set_rgb(r, g, b);
}

void leds_init(void) {
    DDRD = (1 << PD3) | (1 << PD6) | (1 << PD5);

    TCCR0A |= (1 << WGM00) | (1 << WGM01); // Fast PWM mode
    TCCR0B |= (1 << CS02);                 // Prescaler = 256
    // clear on compare match, set at BOTTOM so = 1
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
    // OCOB = red led, OCOA = green led
    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Fast PWM mode
    TCCR2A |= (1 << COM2B1);               // blue led
    TCCR2B |= (1 << CS22) | (1 << CS21);   // Prescaler = 256
}

int main(void) {
    leds_init();
    uart_init();

    const uint8_t max_prompt_size = 7;
    char prompt[max_prompt_size + 1];

    while (1) {
        uart_printstr("Enter a color: ");
        enter_prompt(prompt, max_prompt_size);
        convert_hex_to_rgb(prompt);
    }

    return 0;
}
