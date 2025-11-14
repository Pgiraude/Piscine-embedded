#ifndef MAIN_H
#define MAIN_H

#include <avr/eeprom.h>
#include <avr/io.h>

#define MAX_PROMPT_SIZE 50
#define START_NUMBER 200
#define SEPARATOR_NUMBER 201
#define STOP_NUMBER 202

#define MAX_KEY_VALUE_SIZE 32
#define MAX_MEM_SIZE (MAX_KEY_VALUE_SIZE + 3)

typedef enum { PARSING_ERROR = -1, OK, EEPROM_MATCH, EEPROM_MISMATCH } t_status;

typedef enum { NOT_FOUND, READ, WRITE, FORGET, PRINT } t_command;

typedef struct {
    t_status status;
    uint16_t address;
    uint8_t value;
} ParsedPromptResult;

typedef enum {
    ERROR_1 = (1 << PB0),
    ERROR_2 = (1 << PB1),
    ERROR_3 = (1 << PB2),
    ERROR_4 = (1 << PB0) | (1 << PB1)
} t_error;

void uart_init(void);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);
void uart_printstr_uint(const uint8_t *str);
void uart_printvisiblestr(const uint8_t *str);
uint8_t *enter_prompt(uint8_t *prompt, uint16_t max_prompt_size);
void uart_printint(const uint32_t value);

int8_t get_hex_index(char c);
int32_t convert_hex_to_decimal(uint8_t *input, uint8_t end);
int8_t convert_decimal_to_hex(const uint16_t value, uint8_t *hex_str,
                              uint8_t hex_size, uint8_t nbr_digits);
int8_t ft_itoa(const uint32_t value, char *str, uint8_t str_size);
uint16_t ft_strlen(uint8_t *str);
int8_t ft_strcmp(const uint8_t *str1, const uint8_t *str2);
uint8_t is_visible_char(uint8_t c);

ParsedPromptResult parse_and_check_eeprom(char *prompt);

void print_eeprom(void);
t_command parse_command(uint8_t *prompt);
int8_t write_eeprom(uint8_t *prompt);

int8_t forget_eeprom(uint8_t *prompt);

void ft_error(t_error error);

int16_t get_iner_str(uint8_t *str_to_extract, uint8_t *result_str,
                     uint8_t sep_1, uint8_t sep_2);

int16_t find_stop_number(void);

int8_t parse_key(uint8_t *prompt, uint8_t *key);

int8_t read_eeprom(uint8_t *prompt);

#endif