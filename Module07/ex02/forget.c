#include "main.h"

int8_t parse_key(uint8_t *prompt, uint8_t *key) {

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
    i += res;
    if (prompt[i] == '\0') {
        return PARSING_ERROR;
    }
    return OK;
}

void move_block_left(uint8_t *copy_eeprom, uint16_t from, uint16_t to,
                     uint16_t dst) {
    uint16_t len = to - from + 1;

    for (uint16_t i = 0; i < len; i++) {
        eeprom_write_byte((uint8_t *)(dst + i), copy_eeprom[from + i]);
    }
}

int16_t loop(uint8_t *buffer, uint8_t *key) {
    uint8_t copy_eeprom[1024];
    eeprom_read_block(copy_eeprom, 0, 1024);

    uint8_t *ptr = copy_eeprom;
    int16_t i = 0;
    while (i < 1023) {

        int16_t len =
            get_iner_str(ptr + i, buffer, START_NUMBER, SEPARATOR_NUMBER);

        if (len == PARSING_ERROR) {
            uart_printstr("Not found ^^\r\n");
            return PARSING_ERROR;
        }

        i += len;
        if (ft_strcmp(key, buffer)) {
            int16_t last_val = i;
            while (copy_eeprom[i] != START_NUMBER &&
                   copy_eeprom[i] != STOP_NUMBER) {
                i++;
            }
            while (last_val > 0 && copy_eeprom[last_val] != START_NUMBER) {
                last_val--;
            }

            int16_t stop_pos = find_stop_number();
            move_block_left(copy_eeprom, i, stop_pos, last_val);
            return i;
        }
    }
    return OK;
}

int8_t forget_eeprom(uint8_t *prompt) {
    uint8_t buffer[MAX_MEM_SIZE];
    uint8_t key[MAX_MEM_SIZE];

    if (parse_key(prompt, key) == PARSING_ERROR) {
        uart_printstr("format error\r\n");
        return PARSING_ERROR;
    }

    int16_t i = 0;
    if (eeprom_read_byte((const uint8_t *)i) != START_NUMBER) {
        uart_printstr("Not found\r\n");
        return 0;
    }
    loop(buffer, key);

    return OK;
}