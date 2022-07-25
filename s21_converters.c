#include "s21_decimal.h"

/**
 * Конвертация из int в decimal
 * @return 0 - ОК, 1 - ошибка конвертации
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    int error_code = 0;
    if (dst == NULL) {
        error_code = 1;
    } else {
        unsigned int number = s21_set_sign(src, dst);
        dst->bits[0] = number;
    }
    return error_code;
}

/**
 * Конвертация из float в decimal
 * @return 0 - ОК, 1 - ошибка конвертации
 */
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    int error_code = 1;
    int sign = s21_get_float_sign(&src);
    int exp = s21_get_float_exp(&src);

    if (src != s21_INF && src != s21_NEGATIVE_INF && src != s21_NAN && src != 0 && dst) {
        double positive_float = (double)fabs(src);
        int scale = 0;
        for (; scale < 28 && (int)positive_float / (int)pow(2, 21) == 0; positive_float *= 10, scale++) {}
        positive_float = round(positive_float);
        if (scale <= 28 && (exp > -94 && exp < 96)) {
            floatbits mantiss;
            positive_float = (float)positive_float;
            for (; fmod(positive_float, 10) == 0 && scale > 0; scale--, positive_float /= 10) {}
            mantiss.float_part = positive_float;
            exp = s21_get_float_exp(&mantiss.float_part);
            dst->bits[exp / 32] |= 1 << exp % 32;
            for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
                if ((mantiss.integer_part & (1 << j)) != 0) {
                    dst->bits[i / 32] |= 1 << i % 32;
                }
            }
            dst->bits[3] = (sign << 31) | (scale << 16);
            error_code = 0;
        }
    }
    return error_code;
}

/**
 * Конвертация из decimal в int
 * @return 0 - ОК, 1 - ошибка конвертации
 */
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int sign = s21_get_bit(src.bits[3], 31);

    s21_decimal int_number = {{0, 0, 0, 0}};
    s21_truncate(src, &int_number);
    *dst = (int) src.bits[0];

    *dst *= sign ? -1 : 1;
    return 0;
}

/**
 * Конвертация из decimal во float
 * @return 0 - ОК, 1 - ошибка конвертации
 */
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int error_code = 1;
    if (dst != NULL) {
        double temp = 0;
        int scale = 0;
        for (int i = 0; i < 96; i++) {
            if ((src.bits[i / 32] & (1 << i % 32)) != 0) {
                temp += pow(2, i);
            }
        }
        if ((scale = (src.bits[3] & ~0x80000000) >> 16) > 0) {
            for (int i = scale; i > 0; i--) {
                temp /= 10.0;
            }
        }
        *dst = (float)temp;
        *dst *= src.bits[3] >> 31 ? -1 : 1;
        error_code = 0;
    }
    return error_code;
}
