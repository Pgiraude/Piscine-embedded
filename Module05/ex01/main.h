#ifndef MAIN_H
#define MAIN_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void uart_init(void);
void uart_printstr(const char *str);
char *enter_prompt(char *prompt, uint16_t max_prompt_size);
int8_t convert_decimal_to_hex(const uint16_t value, char *hex_str, uint8_t hex_size, uint8_t nbr_digits);

uint16_t ft_strlen(char *str);

#endif