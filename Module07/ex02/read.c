#include "main.h"

int16_t loop_read(uint8_t *buffer, uint8_t *key) {
    uint8_t copy_eeprom[1024];
    eeprom_read_block(copy_eeprom, 0, 1024);

    uint8_t *ptr = copy_eeprom;
    int16_t i = 0;
    while (i < 1023) {

        int16_t len =
            get_iner_str(ptr + i, buffer, START_NUMBER, SEPARATOR_NUMBER);

        if (len == PARSING_ERROR) {
            uart_printstr("empty\r\n");
            return PARSING_ERROR;
        }

        i += len;
        if (ft_strcmp(key, buffer)) {
            int16_t last_val = i;
            while (copy_eeprom[i] != START_NUMBER &&
                   copy_eeprom[i] != STOP_NUMBER) {
                i++;
            }
            last_val++;
            while (last_val < i) {
                uart_tx(copy_eeprom[last_val++]);
            }
            return OK;
        }
    }
    uart_printstr("empty\r\n");
    return PARSING_ERROR;
}

int8_t read_eeprom(uint8_t *prompt) {
    uint8_t buffer[MAX_MEM_SIZE];
    uint8_t key[MAX_MEM_SIZE];

    if (parse_key(prompt, key) == PARSING_ERROR) {
        uart_printstr("format error\r\n");
        return PARSING_ERROR;
    }

    int16_t i = 0;
    if (eeprom_read_byte((const uint8_t *)i) != START_NUMBER) {
        uart_printstr("empty\r\n");
        return 0;
    }
    loop_read(buffer, key);

    return OK;
}