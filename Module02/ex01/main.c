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

// interrupt vector => TIMER1_COMPA_vect
ISR(TIMER1_COMPA_vect){
    uart_printstr("Hello, World!\r\n");
}

int main(void) {
    uart_init();

    TCCR1B |= (1 << WGM12); // MODE CTC TOP => clear timer on compare match (OCR1A)
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler = 1024

    TIMSK1 |= (1 << OCIE1A); // Enable compare match interrupt
    // When on TIFR1 the OCF1A flag is set => interrupt is triggered
    // OCF1A on TIFR1 is automatically set when clock value (TCNT1) match compare register (OCR1A)
    // OCF1A is cleared when compare match A interrupt vector executed

    OCR1A = PERIOD * 2;
    sei();

    while (1) {}
    return 0;
}
