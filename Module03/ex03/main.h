#ifndef MAIN_H
#define MAIN_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void uart_init(void);
void uart_printstr(const char *str);
char *enter_prompt(char *prompt, uint16_t max_prompt_size);

uint16_t ft_strlen(char *str);

#endif