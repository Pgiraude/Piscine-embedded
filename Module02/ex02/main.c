#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PERIOD 15625 // prescaler = 1024 => 15625 Hz
#define F_CPU 16000000UL
#define BAUD_RATE 115200
#define MYUBRR ((F_CPU / (8UL * BAUD_RATE)) - 1) // Asynchronous Double Speed mode

void uart_tx(unsigned char c){
    // UDRE0 = 1 => transmit buffer is empty
    // UDRE0 = 0 => transmit buffer is full
    while (!(UCSR0A & (1 << UDRE0))) // Wait for empty transmit buffer
        ;
    UDR0 = c; // Put data into buffer, sends the data
}

void uart_printstr(const char *str){
    while (*str) {
        uart_tx(*str++);
    }
}

unsigned char uart_rx(void){
    while (!(UCSR0A & (1 << RXC0))) // Wait for receive complete
        ;
    return UDR0; // Get received data
}

void uart_init(void){
    // Set baud rate
    UCSR0A |= (1 << U2X0); // Double Speed mode
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;

    UCSR0B |= (1 << TXEN0); // Enable transmitter
    UCSR0B |= (1 << RXEN0); // Enable receiver

    // format frame 8N1 : 8-bit, 1-stop bit, no parity cf UCSRnC datasheet
    // default (1 << UCSZ00) | (1 << UCSZ01) on 1 => 8-bit data
    UCSR0C =  (1 << UCSZ00) | (1 << UCSZ01); 
}

int main(void) {
    uart_init();

    while (1) {
        unsigned char data = uart_rx();
        uart_tx(data);
    }
    return 0;
}
