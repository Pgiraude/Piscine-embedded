#include "main.h"

int8_t parse_key_value(uint8_t *prompt, uint8_t *key, uint8_t *value) {

    uint8_t i = 0;
    while (prompt[i] && prompt[i] != ' ') {
        i++;
    }
    while (prompt[i] && prompt[i] == ' ') {
        i++;
    }
    if (prompt[i] != '"') {
        return PARSING_ERROR;
    }
    int16_t res = get_iner_str(prompt + i, key, '"', '"');
    if (res == PARSING_ERROR) {
        return PARSING_ERROR;
    }
    i += res + 1;
    while (prompt[i] && prompt[i] == ' ') {
        i++;
    }
    if (prompt[i] != '"') {
        return PARSING_ERROR;
    }
    res = get_iner_str(prompt + i, value, '"', '"');
    if (res == PARSING_ERROR) {
        return PARSING_ERROR;
    }
    i += res + 1;
    if (prompt[i] != '\0') {
        return PARSING_ERROR;
    }

    return OK;
}

void write_next_key_value(uint8_t *key, uint8_t *value, uint16_t address) {
    uint16_t i = 0;
    eeprom_write_byte((uint8_t *)address + i++, START_NUMBER);
    while (*key) {
        eeprom_write_byte((uint8_t *)address + i++, *key);
        key++;
    }
    eeprom_write_byte((uint8_t *)address + i++, SEPARATOR_NUMBER);
    while (*value) {
        eeprom_write_byte((uint8_t *)address + i++, *value);
        value++;
    }
    eeprom_write_byte((uint8_t *)address + i, STOP_NUMBER);
}

int16_t loop_write(uint8_t *key) {
    uint8_t buffer[MAX_KEY_VALUE_SIZE];
    uint8_t copy_eeprom[1024];
    eeprom_read_block(copy_eeprom, 0, 1024);

    uint8_t *ptr = copy_eeprom;
    int16_t i = 0;
    while (i < 1023) {

        int16_t len =
            get_iner_str(ptr + i, buffer, START_NUMBER, SEPARATOR_NUMBER);

        if (len == PARSING_ERROR) {
            return OK;
        }

        i += len;
        if (ft_strcmp(key, buffer)) {
            return PARSING_ERROR;
        }
    }
    return OK;
}

int8_t write_eeprom(uint8_t *prompt) {

    uint8_t key[MAX_PROMPT_SIZE + 1];
    uint8_t value[MAX_PROMPT_SIZE + 1];

    if (parse_key_value(prompt, key, value) == PARSING_ERROR) {
        uart_printstr("Format error\r\n");
        return PARSING_ERROR;
    }
    uint16_t len = ft_strlen(key) + ft_strlen(value);
    if (len > MAX_KEY_VALUE_SIZE) {
        uart_printstr("Error: key/value to big\r\n");
        return PARSING_ERROR;
    }

    int16_t addr_next_save = find_stop_number();
    if (addr_next_save < 0) {
        uart_printstr("Corrupt data\r\n");
        return PARSING_ERROR;
    }
    if (addr_next_save + len + 2 > 1023) {
        uart_printstr("No space left");
    }

    if (loop_write(key) == PARSING_ERROR) {
        uart_printstr("already exists\r\n");
        return PARSING_ERROR;
    }

    write_next_key_value(key, value, addr_next_save);

    uint8_t buffer[20];
    convert_decimal_to_hex(addr_next_save, buffer, 10, 7);
    uart_printstr_uint(buffer);
    uart_printstr("\r\n");

    return OK;
}