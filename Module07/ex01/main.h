#ifndef MAIN_H
#define MAIN_H

#include <avr/eeprom.h>
#include <avr/io.h>

#define MAX_PROMPT_SIZE 15

typedef enum {
    PARSING_ERROR = -1,
    EEPROM_MATCH = 0,
    EEPROM_MISMATCH = 1
} t_status;

typedef struct {
    t_status status;
    uint16_t address;
    uint8_t value;
} ParsedPromptResult;

void uart_init(void);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);
char *enter_prompt(char *prompt, uint16_t max_prompt_size);

int8_t get_hex_index(char c);
int32_t convert_hex_to_decimal(char *input, uint8_t end);
int8_t convert_decimal_to_hex(const uint16_t value, char *hex_str,
                              uint8_t hex_size, uint8_t nbr_digits);
int8_t ft_itoa(const uint32_t value, char *str, uint8_t str_size);
uint16_t ft_strlen(char *str);

ParsedPromptResult parse_and_check_eeprom(char *prompt);

#endif