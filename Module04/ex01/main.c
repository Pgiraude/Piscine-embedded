#include "main.h"
#define TOP_VALUE 1024
#define LED_CYCLES ((16000000UL / (1024UL * 510)) / 2)
#define STEPS (TOP_VALUE / LED_CYCLES)

ISR_TC0_OVERFLOW {
    static int8_t direction = 1;
    static uint32_t cycle = 0;

    OCR1A = STEPS * cycle;
    cycle += direction;
    if (cycle == LED_CYCLES || cycle == 0) {
        direction = -direction;
    }
}

void set_timer0(void) {
    // mode PWM 1
    TCCR0A |= (1 << WGM00);
    // prescaler 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);
    // Foc = Fclk / N * 510 = 30 Hz
    TIMSK0 |= (1 << TOIE0);
}

void set_timer1(void) {
    // compar match OC1A mode toggle
    TCCR1A |= (1 << COM1A1);
    // compare match value
    OCR1A = 0;
    // mode fast PWM 14
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    // prescaler 256
    TCCR1B |= (1 << CS12);
    // Top value
    ICR1 = TOP_VALUE;
    // Foc = Fclk / N * (Top value + 1) ~= 61 Hz
}

int main(void) {
    set_timer0();
    DDRB |= (1 << PB1);
    set_timer1();

    sei();

    while (1) {
    }

    return 0;
}
