#include "main.h"

int32_t parsing_address(char *prompt) {
    char buffer[MAX_PROMPT_SIZE + 1];

    uint8_t i = 0;
    while (prompt[i] == '0' && i < MAX_PROMPT_SIZE) {
        i++;
    }
    uint8_t index_buffer = 0;
    while (prompt[i] != ' ' && i < MAX_PROMPT_SIZE) {
        buffer[index_buffer] = prompt[i];
        i++;
        index_buffer++;
    }
    buffer[index_buffer] = '\0';

    if (ft_strlen(buffer) > 3) {
        return -1;
    }

    int32_t value = 0;
    value = convert_hex_to_decimal(buffer, 0);
    if (value < 0 || value > 1023) {
        return -1;
    }
    return value;
}

int16_t parsing_byte(char *prompt) {
    char buffer[MAX_PROMPT_SIZE + 1];

    uint8_t i = 0;
    while (prompt[i] != ' ' && i < MAX_PROMPT_SIZE) {
        i++;
    }
    while (prompt[i] == ' ' && i < MAX_PROMPT_SIZE) {
        i++;
    }
    uint8_t index_buffer = 0;
    while (prompt[i] && i < MAX_PROMPT_SIZE) {
        buffer[index_buffer] = prompt[i];
        i++;
        index_buffer++;
    }
    buffer[index_buffer] = '\0';

    if (ft_strlen(buffer) > 2) {
        return -1;
    }
    int16_t value = 0;
    value = convert_hex_to_decimal(buffer, 0);
    if (value < 0 || value > 255) {
        return -1;
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
    if (address == -1) {
        res.status = -1;
        return res;
    }
    res.address = address;

    int16_t value = 0;
    value = parsing_byte(prompt);
    if (value == -1) {
        res.status = -1;
        return res;
    }
    res.value = value;

    uint8_t cur_byte_val = 0;
    cur_byte_val = eeprom_read_byte((const uint8_t *)res.address);
    res.status = (cur_byte_val == value) ? 0 : 1;

    return res;
}