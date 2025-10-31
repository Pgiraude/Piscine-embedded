#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD_RATE 115200
#define MYUBRR ((F_CPU / (8UL * BAUD_RATE)) - 1) // Asynchronous Double Speed mode


void uart_tx(unsigned char c){
    // Wait for empty transmit buffer
    // UDRE0 = 1 => transmit buffer is empty
    // UDRE0 = 0 => transmit buffer is full
    while (!(UCSR0A & (1 << UDRE0)))
    ;
    // Put data into buffer, sends the data
    UDR0 = c;
}

void uart_init(void){
    // Set baud rate
    UCSR0A |= (1 << U2X0); // Double Speed mode
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;

    // Enable transmitter
    UCSR0B |= (1 << TXEN0);
    // Enable receiver
    UCSR0B |= (1 << RXEN0);

    // format frame 8N1 : 8-bit, 1-stop bit, no parity cf UCSRnC datasheet
    // default (1 << UCSZ00) | (1 << UCSZ01) on 1 => 8-bit data
    UCSR0C =  (1 << UCSZ00) | (1 << UCSZ01); 
}

int main(void) {
    uart_init();

    while (1) {
        uart_tx('Z');
        _delay_ms(1000); // 1 Hz = 1 send every second
    }

    return 0;
}

