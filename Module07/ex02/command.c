#include "main.h"

void print_eeprom(void) {
    uint8_t buffer[10];
    uint8_t to_print[17];
    uint8_t value = 0;

    for (uint16_t i = 0; i < 1024; i++) {
        if (i % 16 == 0) {
            if (i != 0) {
                to_print[16] = '\0';
                uart_printstr(" |");
                uart_printvisiblestr(to_print);
                uart_printstr("|\r\n");
            }
            convert_decimal_to_hex(i, buffer, 10, 7);
            uart_printstr_uint(buffer);
            uart_printstr(" ");
        } else if (i % 2 == 0) {
            uart_printstr(" ");
        }
        value = eeprom_read_byte((const uint8_t *)i);
        to_print[i % 16] = value;
        convert_decimal_to_hex(value, buffer, 10, 2);
        if (value == START_NUMBER) {
            uart_printstr("\033[33m");
        } else if (value == SEPARATOR_NUMBER) {
            uart_printstr("\033[34m");
        } else if (value == STOP_NUMBER) {
            uart_printstr("\033[32m");
        }
        uart_printstr_uint(buffer);
        if (value == START_NUMBER || value == SEPARATOR_NUMBER ||
            value == STOP_NUMBER) {
            uart_printstr("\033[0m");
        }
    }
    to_print[16] = '\0';
    uart_printstr(" |");
    uart_printvisiblestr(to_print);
    uart_printstr("|\r\n");
}

int16_t get_iner_str(uint8_t *str_to_extract, uint8_t *result_str,
                     uint8_t sep_1, uint8_t sep_2) {
    uint8_t i = 0;
    while (str_to_extract[i] && str_to_extract[i] != sep_1) {
        i++;
    }
    i++;
    uint8_t index_result = 0;
    while (str_to_extract[i] && str_to_extract[i] != sep_2) {
        result_str[index_result++] = str_to_extract[i++];
    }
    result_str[index_result] = '\0';
    if (!index_result || str_to_extract[i] != sep_2) {
        return PARSING_ERROR;
    }
    return i;
}

int16_t is_memory_use(uint16_t start_address) {
    uint16_t i = 0 + start_address;
    uint8_t state_check = 0;

    if (eeprom_read_byte((const uint8_t *)i++) != START_NUMBER) {
        return PARSING_ERROR;
    }
    state_check++;
    uint16_t len = 1;
    while (i < 1023 && is_visible_char(eeprom_read_byte((const uint8_t *)i))) {
        i++;
        len++;
    }
    if (i < 1023 && eeprom_read_byte((const uint8_t *)i) == SEPARATOR_NUMBER) {
        state_check++;
        i++;
        len++;
    }

    while (i < 1023 && is_visible_char(eeprom_read_byte((const uint8_t *)i))) {
        i++;
        len++;
    }
    if (i < 1023 && (eeprom_read_byte((const uint8_t *)i) == START_NUMBER ||
                     eeprom_read_byte((const uint8_t *)i) == STOP_NUMBER)) {
        state_check++;
        len++;
    }
    if (len <= MAX_MEM_SIZE) {
        state_check++;
    }

    if (state_check == 4) {
        return i;
    } else {
        return PARSING_ERROR;
    }
}

int16_t find_stop_number(void) {
    int16_t i = 0;

    if (eeprom_read_byte((const uint8_t *)i) != START_NUMBER) {
        return i;
    }

    while (i < 1023 && (eeprom_read_byte((const uint8_t *)i) != STOP_NUMBER)) {
        i = is_memory_use(i);
        if (i < 0) {
            return PARSING_ERROR;
        }
    }
    return i;
}

t_command parse_command(uint8_t *prompt) {
    uint8_t buffer[MAX_PROMPT_SIZE + 1];

    uint8_t i = 0;
    while (prompt[i] && prompt[i] != ' ') {
        buffer[i] = prompt[i];
        i++;
    }
    buffer[i] = '\0';

    if (ft_strcmp(buffer, (uint8_t *)"WRITE"))
        return WRITE;
    if (ft_strcmp(buffer, (uint8_t *)"READ"))
        return READ;
    if (ft_strcmp(buffer, (uint8_t *)"FORGET"))
        return FORGET;
    if (ft_strcmp(buffer, (uint8_t *)"PRINT"))
        return PRINT;

    return NOT_FOUND;
}
