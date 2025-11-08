#ifndef MAIN_H
#define MAIN_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void uart_init(void);
unsigned char uart_rx(void);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);
int16_t convert_hex_to_decimal(char *input, uint8_t end);

#endif