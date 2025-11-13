#include "main.h"

typedef enum {
    ERROR_1 = (1 << PB0),
    ERROR_2 = (1 << PB1),
    ERROR_3 = (1 << PB2),
    ERROR_4 = (1 << PB0) | (1 << PB1)
} t_error;

void ft_error(t_error error) {
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    PORTB |= (error & 0x07);

    TWCR = 0;
    while (1)
        ;
}

//  100,000 write/erase cycle
unsigned char EEPROM_read(uint16_t uiAddress) {
    /* Wait for completion of previous write */
    while (EECR & (1 << EEPE))
        ;
    /* Set up address register */
    EEAR = uiAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1 << EERE);
    /* Return data from Data Register */
    return EEDR;
}

void print_eeprom_status(uint16_t change_octet, uint8_t highlight_change) {
    char buffer[10];
    uint8_t value = 0;

    for (uint16_t i = 0; i < 1024; i++) {
        if (i % 16 == 0) {
            if (i != 0) {
                uart_printstr("\r\n");
            }
            convert_decimal_to_hex(i, buffer, 10, 7);
            uart_printstr(buffer);
            uart_printstr(" ");
        } else if (i % 2 == 0) {
            uart_printstr(" ");
        }
        value = EEPROM_read(i);
        convert_decimal_to_hex(value, buffer, 10, 2);
        if (highlight_change && change_octet == i) {
            uart_printstr("\033[31m");
        }
        uart_printstr(buffer);
        if (highlight_change && change_octet == i) {
            uart_printstr("\033[0m");
        }
    }
    uart_printstr("\r\n");
}

// EEPROM size = 1Kbytes => 1024 octets for Atmega328P cf p294
int main(void) {
    ParsedPromptResult res;

    uart_init();
    char prompt[MAX_PROMPT_SIZE + 1];

    while (1) {
        enter_prompt(prompt, MAX_PROMPT_SIZE);
        res = parse_and_check_eeprom(prompt);
        if (res.status == PARSING_ERROR) {
            uart_printstr("incorrect format\r\n");
        } else if (res.status == EEPROM_MISMATCH) {
            eeprom_write_byte((uint8_t *)res.address, res.value);
            print_eeprom_status(res.address, 1);
        } else {
            uart_printstr("~nothing happen~\r\n");
        }
    }

    return 0;
}
