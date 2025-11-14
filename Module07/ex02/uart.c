#include "main.h"

#define F_CPU 16000000UL
#define BAUD_RATE 115200
#define MYUBRR ((F_CPU / (8UL * BAUD_RATE)) - 1)

void uart_init(void) {
    UCSR0A |= (1 << U2X0); // Double Speed mode
    // Set baud rate
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;

    UCSR0B |= (1 << TXEN0); // Enable transmitter
    UCSR0B |= (1 << RXEN0); // Enable receiver
    // format frame 8N1 : 8-bit, 1-stop bit, no parity cf UCSRnC datasheet
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

unsigned char uart_rx(void) {
    while (!(UCSR0A & (1 << RXC0))) // Wait for receive complete
        ;
    return UDR0; // Get received data
}

void uart_tx(unsigned char c) {
    while (!(UCSR0A & (1 << UDRE0))) {
    }         // Wait for empty transmit buffer
    UDR0 = c; // Put data into buffer, sends the data
}

void uart_printstr(const char *str) {
    while (*str) {
        uart_tx(*str++);
    }
}

void uart_printstr_uint(const uint8_t *str) {
    while (*str) {
        uart_tx(*str++);
    }
}

void uart_printvisiblestr(const uint8_t *str) {
    while (*str) {
        if (*str >= 32 && *str <= 126) {
            uart_tx(*str); // caractère affichable
        } else {
            uart_tx(' ');
        }
        str++;
    }
}

void uart_printint(const uint32_t value) {
    char str[100];

    ft_itoa(value, str, 100);
    uart_printstr(str);
}

void uart_consume_escape_sequence(void) {
    uint8_t next = uart_rx();
    if (next != '[' && next != 'O') {
        return;
    }
    while (1) {
        uint8_t c = uart_rx();

        if (c >= 64 && c <= 126) {
            return;
        }
    }
}

uint8_t *enter_prompt(uint8_t *prompt, uint16_t max_prompt_size) {
    uint16_t index = 0;

    uint8_t c = 0;
    while (c != '\r') {
        c = uart_rx();
        if (c == 27) {
            uart_consume_escape_sequence();
            continue;
        }
        if (c == 127 || c == 8) {
            if (index > 0) {
                index--;
                uart_printstr("\b \b");
            }
        } else if (index < max_prompt_size && c >= 32 && c <= 126) {
            prompt[index] = c;
            index++;
            uart_tx(c);
        }
    }
    prompt[index] = '\0';
    uart_printstr("\r\n");

    return prompt;
}