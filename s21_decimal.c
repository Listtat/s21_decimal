#include "s21_decimal.h"

void s21_print_decimal(int number, char *str) {
    for (int i = 0, j = 31; i < 32; i++, j--) {
        str[j] = s21_get_bit(number, i) + '0';
    }
    str[32] = '\0';
    printf("%s\n", str);
}









