#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD_RATE 115200
#define MYUBRR ((F_CPU / (16UL * BAUD_RATE)) - 1) // Asynchronous Normal mode

unsigned char toggle_case(unsigned char c){
    const int diff = 'a' - 'A';

    if (c >= 'a' && c <= 'z') {
        return c - diff;
    }
    else if (c >= 'A' && c <= 'Z') {
        return c + diff;
    }
    return c;
}

void uart_tx(unsigned char c){
    while (!(UCSR0A & (1 << UDRE0))) {} // Wait for empty transmit buffer
    UDR0 = c; // Put data into buffer, sends the data
}

void uart_printstr(const char *str){
    while (*str) {
        uart_tx(*str++);
    }
}

void uart_init(void){
    // Set baud rate
    const int ubrr = MYUBRR + 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0B |= (1 << TXEN0); // Enable transmitter
    UCSR0B |= (1 << RXEN0); // Enable receiver
    UCSR0B |= (1 << RXCIE0); // Enable receive complete interrupt when RXC0 flag is set on UCSR0A
    // RXC0 set on UCSR0A when there are unread data in the receive buffer and cleared when the receive buffer is empty

    // format frame 8N1 : 8-bit, 1-stop bit, no parity cf UCSRnC datasheet
    // default (1 << UCSZ00) | (1 << UCSZ01) on 1 => 8-bit data
    UCSR0C =  (1 << UCSZ00) | (1 << UCSZ01); 
}

// interrupt service routine for receive complete
ISR(USART_RX_vect){
    uart_tx(toggle_case(UDR0));
}

int main(void) {
    uart_init();

    sei();
    while (1) {}
    return 0;
}
