#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD_RATE 115200
#define MYUBRR ((F_CPU / (16UL * BAUD_RATE)) - 1) // Asynchronous Normal mode

#define TEST "test"
#define MODE_USERNAME 0
#define MODE_PASSWORD 1

unsigned char uart_rx(void){
    while (!(UCSR0A & (1 << RXC0))) // Wait for receive complete
        ;
    return UDR0; // Get received data
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

int enter_input(int mode){

    if (MODE_USERNAME == mode) {
        uart_printstr("username: ");
    } else if (mode == MODE_PASSWORD) {
        uart_printstr("password: ");
    }

    unsigned char c = 0;
    while (c != '\r') {
        c = uart_rx();
        if (MODE_USERNAME == mode) {
            uart_tx(c);
        } else {
            uart_tx('*');
        }
    }
    uart_printstr("\r\n");

    return 0;
}

void uart_init(void){
    // Set baud rate
    const int ubrr = MYUBRR + 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0B |= (1 << TXEN0); // Enable transmitter
    UCSR0B |= (1 << RXEN0); // Enable receiver

    // format frame 8N1 : 8-bit, 1-stop bit, no parity cf UCSRnC datasheet
    // default (1 << UCSZ00) | (1 << UCSZ01) on 1 => 8-bit data
    UCSR0C =  (1 << UCSZ00) | (1 << UCSZ01); 
}


int main(void) {
    uart_init();

    while (1) {
        enter_input(MODE_USERNAME);
        enter_input(MODE_PASSWORD);

    }
    return 0;
}
