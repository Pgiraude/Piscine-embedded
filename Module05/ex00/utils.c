#include "main.h"

uint16_t ft_strlen(char *str) {
    if (!str) {
        return 0;
    }
    uint16_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int8_t ft_strcmp(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    if (*str1 == '\0' && *str2 == '\0') {
        return 1;
    }
    return 0;
}

const char hex_chars_upper[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const char hex_chars_lower[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

int8_t get_hex_index(char c) {
    uint8_t index = 0;
    while (hex_chars_upper[index]) {
        if (hex_chars_upper[index] == c) {
            return index;
        }
        index++;
    }
    index = 0;
    while (hex_chars_lower[index]) {
        if (hex_chars_lower[index] == c) {
            return index;
        }
        index++;
    }
    return -1;
}

void convert_decimal_to_hex(const uint16_t value, char *hex_str) {
    if (value == 0) {
        hex_str[0] = '0';
        hex_str[1] = '\0';
        return;
    }

    uint16_t val = value;
    uint16_t len = 0;

    while (val > 0) {
        val /= 16;
        len++;
    }
    val = value;
    int16_t index = len - 1;
    while (index >= 0) {
        hex_str[index] = hex_chars_upper[val % 16];
        val /= 16;
        index--;
    }
    hex_str[len] = '\0';
}

int16_t convert_hex_to_decimal(char *input, uint8_t end) {
    const uint8_t len = ft_strlen(input);
    if (end > len || len == 0) {
        return -1;
    }
    const uint8_t cycles = end ? end : len;

    int16_t value = 0;
    uint8_t index = 0;
    while (index < cycles) {
        if (get_hex_index(input[index]) == -1) {
            return -1;
        }
        value = value * 16 + get_hex_index(input[index]);
        index++;
    }
    return value;
}