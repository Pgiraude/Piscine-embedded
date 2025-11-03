#include <avr/io.h>
#include <util/delay.h>
#define PRESCALER 256 // prescaler = 1024 => 15625 Hz
#define F_CPU 16000000UL
#define DUTY_CYCLE (F_CPU / (PRESCALER * 256))

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0A = r;
    OCR0B = g;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main(void) {
    DDRD = (1 << PD3) | (1 << PD6) | (1 << PD5);

    TCCR0A |= (1 << WGM00) | (1 << WGM01); // Fast PWM mode
    TCCR0B |= (1 << CS02);                 // Prescaler = 256
    // clear on compare match, set at BOTTOM so = 1
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);

    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Fast PWM mode
    TCCR2A |= (1 << COM2B1);
    TCCR2B |= (1 << CS22) | (1 << CS21); // Prescaler = 256

    int i = -1;
    while (1) {
        while (++i < 255) {
            wheel(i);
            _delay_ms(10);
        }
        while (--i > 0) {
            wheel(i);
            _delay_ms(10);
        }
    }

    return 0;
}
