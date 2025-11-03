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