#include "main.h"

int32_t parsing_address(char *prompt) {
    char buffer[MAX_PROMPT_SIZE + 1];

    uint8_t i = 0;
    while (prompt[i] && prompt[i] == '0') {
        i++;
    }
    uint8_t index_buffer = 0;
    while (prompt[i] && prompt[i] != ' ') {
        buffer[index_buffer] = prompt[i];
        i++;
        index_buffer++;
    }
    if (i > 0 && index_buffer == 0) {
        buffer[index_buffer++] = '0';
    }
    buffer[index_buffer] = '\0';

    if (ft_strlen(buffer) > 3) {
        return PARSING_ERROR;
    }

    int32_t value = 0;
    value = convert_hex_to_decimal(buffer, 0);
    if (value < 0 || value > 1023) {
        return PARSING_ERROR;
    }
    return value;
}

int16_t parsing_byte(char *prompt) {
    char buffer[MAX_PROMPT_SIZE + 1];

    uint8_t i = 0;
    while (prompt[i] && prompt[i] != ' ') {
        i++;
    }
    while (prompt[i] && prompt[i] == ' ') {
        i++;
    }
    uint8_t index_buffer = 0;
    while (prompt[i] && prompt[i]) {
        buffer[index_buffer] = prompt[i];
        i++;
        index_buffer++;
    }
    buffer[index_buffer] = '\0';

    if (ft_strlen(buffer) > 2) {
        return PARSING_ERROR;
    }
    int16_t value = 0;
    value = convert_hex_to_decimal(buffer, 0);
    if (value < 0 || value > 255) {
        return PARSING_ERROR;
    }
    return value;
}

ParsedPromptResult parse_and_check_eeprom(char *prompt) {
    ParsedPromptResult res;
    res.status = 0;
    res.address = 0;
    res.value = 0;

    int32_t address = 0;
    address = parsing_address(prompt);
    if (address == PARSING_ERROR) {
        res.status = PARSING_ERROR;
        return res;
    }
    res.address = address;

    int16_t value = 0;
    value = parsing_byte(prompt);
    if (value == PARSING_ERROR) {
        res.status = PARSING_ERROR;
        return res;
    }
    res.value = value;

    uint8_t cur_byte_val = 0;
    cur_byte_val = eeprom_read_byte((const uint8_t *)res.address);
    res.status = (cur_byte_val == value) ? EEPROM_MATCH : EEPROM_MISMATCH;

    return res;
}