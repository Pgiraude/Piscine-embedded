#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD_RATE 115200
#define MYUBRR ((F_CPU / (16UL * BAUD_RATE)) - 1) // Asynchronous Normal mode

#define USERNAME "test"
#define PASSWORD "1234"
#define EXIT "EXIT"
#define MODE_USERNAME 0
#define MODE_PASSWORD 1
#define MODE_EXIT 2

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

void uart_consume_escape_sequence(void)
{
    unsigned char next = uart_rx();

    if (next != '[' && next != 'O') {
        return;
    }
    while (1) {
        unsigned char c = uart_rx();

        if (c >= 64 && c <= 126) {
            return;
        }
    }
}

int ft_strcmp(const char *str1, const char *str2){
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    if (*str1 == '\0' && *str2 == '\0') {
        return 1;
    }
    return 0;
}

int enter_input(int mode, const char *expected_input){
    int index = 0;
    const int max_input_size = 100;
    char input[max_input_size];

    if (MODE_USERNAME == mode) {
        uart_printstr("\tusername: ");
    } else if (mode == MODE_PASSWORD) {
        uart_printstr("\tpassword: ");
    }

    unsigned char c = 0;
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
        } else if (index < max_input_size - 1 && c >= 32 && c <= 126) {
            input[index] = c;
            index++;
            if (MODE_PASSWORD == mode) {
                uart_tx('*');
            } else {
                uart_tx(c);
            }
        }
    }
    input[index] = '\0';
    uart_printstr("\r\n");

    if (ft_strcmp(input, expected_input)) {
        return 1;
    }
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
    int is_username_successful = 0;
    int is_password_successful = 0;
    int is_login = 0;

    while (1) {
        uart_printstr("Enter your login:\r\n");
        is_username_successful = enter_input(MODE_USERNAME, USERNAME);
        is_password_successful = enter_input(MODE_PASSWORD, PASSWORD);
        is_login = is_username_successful && is_password_successful;
        if (is_login) {
            uart_printstr("Login successful\r\n");
            uart_printstr("Welcome to the system some guy!\r\n");
            uart_printstr("Sorry, nothing to do here\r\n");
            uart_printstr("To exit, enter EXIT\r\n");
            while (is_login) {
                is_login = !enter_input(MODE_EXIT, EXIT);
                if (!is_login) {
                    uart_printstr("Exiting...\r\n");
                } else {
                    uart_printstr("You don't know how to spell EXIT?\r\n");
                    uart_printstr("Try again bro\r\n");
                    uart_printstr("To exit, enter EXIT\r\n");
                }
            }
            is_login = 0;
            is_username_successful = 0;
            is_password_successful = 0;
        } else {
            uart_printstr("Bad combination username/password\r\n\r\n");
        }
    }

    return 0;
}
