#include <avr/io.h>
#include <util/delay.h>

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    }
    else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    }
    else {
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main(void) {
    DDRD = (1 << PD3) | (1 << PD6) | (1 << PD5);

    while (1) {

    }

    return 0;
}
