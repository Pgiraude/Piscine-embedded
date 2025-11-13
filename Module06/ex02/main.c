#include "main.h"

#define AHT20_ADRESSE 0x38
#define WRITE 0
#define READ 1
#define NB_MEASUREMENTS 3

typedef enum {
    ERROR_1 = (1 << PB0),
    ERROR_2 = (1 << PB1),
    ERROR_3 = (1 << PB2),
    ERROR_4 = (1 << PB0) | (1 << PB1)
} t_error;

typedef enum { INIT_1 = 0xBE, INIT_2 = 0x08, INIT_3 = 0x00 } t_init;
typedef enum { MESURE_1 = 0xAC, MESURE_2 = 0x33, MESURE_3 = 0x00 } t_mesure;

void ft_error(t_error error) {
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    PORTB |= (error & 0x07);

    TWCR = 0;
    while (1)
        ;
}

void print_hex_value(char c) {
    char buffer_size = 10;
    char buffer[buffer_size];
    convert_decimal_to_hex((uint8_t)c, buffer, buffer_size, 2);
    uart_printstr(buffer);
    uart_printstr(" ");
}

uint8_t i2c_status(void) {
    return (TWSR & TW_STATUS_MASK); // ONLY GET STATUS masking 3 first bits
                                    // (prescaler + reserve bits)
}

void i2c_write(uint8_t data) // send data  ? need to put adresse somewere
{
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
    }
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
    }
}

uint8_t i2c_read_ACK(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT))) {
    }
    if (i2c_status() != TW_MR_DATA_ACK)
        ft_error(ERROR_3);
    return TWDR;
}

uint8_t i2c_read_NACK(void) {
    TWCR = (1 << TWINT) | (1 << TWEN); // NACK
    while (!(TWCR & (1 << TWINT))) {
    }
    if (i2c_status() != TW_MR_DATA_NACK)
        ft_error(ERROR_4);
    return TWDR;
}

void i2c_read(uint8_t *tab) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {
    }

    for (int i = 0; i < 7; i++) {
        if (i < 6) {
            tab[i] = i2c_read_ACK();
        } else {
            tab[i] = i2c_read_NACK();
        }
    }
}

void i2c_init(void) {
    // SCLfreq = 16Mkz / (16 + 2 * TWBR * prescaler)
    // pour SCLfreq = 100kHz <=> TWBR = 72
    // prescalar = 1 by default
    TWBR = 72;
}

void i2c_start(void) {
    TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {
    }
    if ((TWSR & TW_STATUS_MASK) != TW_START)
        ft_error(ERROR_1);
}

void i2c_stop(void) { TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); }

float calculate_humidity(uint8_t tab[][7], uint8_t nb_mesures) {
    float sum = 0;

    for (uint8_t i = 0; i < nb_mesures; i++) {
        uint32_t raw = ((uint32_t)tab[i][1] << 16) |
                       ((uint32_t)tab[i][2] << 8) | ((uint32_t)tab[i][3] << 0);
        raw >>= 4;

        float humidity = ((float)raw / 1048576.0f) * 100.0f;
        sum += humidity;
    }

    return sum / (float)nb_mesures;
}

float calculate_temperature(uint8_t tab[][7], uint8_t nb_mesures) {
    float sum = 0;

    for (uint8_t i = 0; i < nb_mesures; i++) {
        uint32_t raw = ((uint32_t)tab[i][3] << 16) |
                       ((uint32_t)tab[i][4] << 8) | ((uint32_t)tab[i][5] << 0);
        raw = raw & 0b11111111111111111111; // mask of 20 bits for the 20 values

        float humidity = ((float)raw / 1048576.0f) * 200.0f - 50.0f;
        sum += humidity;
    }

    return sum / (float)nb_mesures;
}

void print_round_up_significant(float value, char *round_buff) {
    char buffer[20];
    dtostrf(value, 5, 10, buffer);

    int index = 0;
    while (buffer[index] != '.') {
        index++;
    }
    index++;
    float factor = 1;
    int i = 0;
    int is_upper = 0;
    while (buffer[index] == '0') {
        factor *= 10;
        i++;
        index++;
    }
    index++;
    if (buffer[index] > '4') {
        is_upper = 1;
    }

    if (i < 1) {
        float correct = (float)(buffer[index] - 48) / 100;
        dtostrf(value - correct, 5, 1, round_buff);
    } else {
        float correct = (is_upper ? 0 : (1 / factor) * 0.5f);
        dtostrf(value + correct, 5, i, round_buff);
    }
}

int main(void) {

    uint8_t tab[NB_MEASUREMENTS][7];
    char buffer[20];

    uart_init();
    i2c_init();
    _delay_ms(40);

    i2c_start();
    TWDR = (AHT20_ADRESSE << 1) | WRITE;
    i2c_write(INIT_1);
    i2c_write(INIT_2);
    i2c_write(INIT_3); // pas d ACK ici
    i2c_stop();
    _delay_ms(40);

    float value = 0;
    uint8_t index = 0;
    uint8_t mesure_count = 1;
    while (1) {
        i2c_start(); // check if init done
        TWDR = (AHT20_ADRESSE << 1) | WRITE;
        i2c_write(MESURE_1);
        i2c_write(MESURE_2);
        i2c_write(MESURE_3);
        i2c_stop();
        _delay_ms(100); // if ok wait 10ms

        i2c_start();                        // check if init done
        TWDR = (AHT20_ADRESSE << 1) | READ; // send adress and mode
        i2c_read(tab[index]);
        uart_printstr("Temperature: ");
        value = calculate_temperature(tab, mesure_count);

        print_round_up_significant(value, buffer);
        uart_printstr(buffer);
        uart_printstr("C");

        uart_printstr(", Humidity: ");
        value = calculate_humidity(tab, mesure_count);
        print_round_up_significant(value, buffer);
        uart_printstr(buffer);

        uart_printstr("%");
        uart_printstr("\r\n");
        i2c_stop();
        _delay_ms(2000);

        if (mesure_count < 3) {
            mesure_count++;
        }
        index++;
        if (index == NB_MEASUREMENTS)
            index = 0;
    }

    return 0;
}
