#include "main.h"

void ft_error(t_error error) {
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    PORTB |= (error & 0x07);

    TWCR = 0;
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

// EEPROM size = 1Kbytes => 1024 octets for Atmega328P cf p294
int main(void) {

    uart_init();
    uint8_t prompt[MAX_PROMPT_SIZE + 1];

    while (1) {
        enter_prompt(prompt, MAX_PROMPT_SIZE);
        switch (parse_command(prompt)) {
        case PRINT:
            print_eeprom();
            break;
        case WRITE:
            write_eeprom(prompt);
            break;
        case FORGET:
            forget_eeprom(prompt);
            break;
        case READ:
            read_eeprom(prompt);
            break;
        default:
            uart_printstr("\033[31mCommand not found\033[0m\r\n");
        }
    }

    return 0;
}
