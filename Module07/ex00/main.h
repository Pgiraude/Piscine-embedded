#ifndef MAIN_H
#define MAIN_H

#include <avr/eeprom.h>
#include <avr/io.h>

void uart_init(void);
void uart_printstr(const char *str);
char *enter_prompt(char *prompt, uint16_t max_prompt_size);
int8_t convert_decimal_to_hex(const uint16_t value, char *hex_str,
                              uint8_t hex_size, uint8_t nbr_digits);
int8_t ft_itoa(const uint32_t value, char *str, uint8_t str_size);

uint16_t ft_strlen(char *str);

void uart_tx(unsigned char c);

#endif