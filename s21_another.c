#include "s21_decimal.h"

//  Округляет указанное Decimal число до ближайшего целого числа в сторону отрицательной бесконечности
int s21_floor(s21_decimal value, s21_decimal *result) {
    int value_scale = s21_get_scale(value.bits[3]);
    int value_sign = s21_get_bit(value.bits[3], 31);
    unsigned int buffer = 0;

    for (int i = 0; i < value_scale; i++) {
        unsigned int remainder = s21_divide_by_10(&value);
        buffer += remainder;

        if (i == value_scale - 1 && value_sign == 1 && buffer != 0) {
            if (value.bits[0] + 1u < value.bits[0]) {
                if (value.bits[1] + 1u < value.bits[1]) {
                    value.bits[2] += 1u;
                } else {
                    value.bits[1] += 1u;
                }
            } else {
                value.bits[0] += 1u;
            }
        }
    }

    *result = value;
    s21_set_bit(&result->bits[3], 31, value_sign);
    s21_set_scale(result, 0);

    return 0;
}

//  Округляет Decimal до ближайшего целого числа
int s21_round(s21_decimal value, s21_decimal *result) {
    int value_scale = s21_get_scale(value.bits[3]);
    int value_sign = s21_get_bit(value.bits[3], 31);

    for (int i = 0; i < value_scale; i++) {
        if (i == value_scale - 1) {
            unsigned int remainder = s21_divide_by_10(&value);

            // Если остаток от деления больше или равен 5 добавляем 1
            if (remainder >= 5) {
                if (value.bits[0] + 1u < value.bits[0]) {
                    if (value.bits[1] + 1u < value.bits[1]) {
                        value.bits[2] += 1u;
                    } else {
                        value.bits[1] += 1u;
                    }
                } else {
                    value.bits[0] += 1u;
                }
            }
        } else {
            s21_divide_by_10(&value);
        }
    }

    *result = value;
    s21_set_bit(&result->bits[3], 31, value_sign);
    s21_set_scale(result, 0);
    return 0;
}

//  Возвращает целые цифры указанного Decimal числа; любые дробные цифры отбрасываются, включая конечные нули
int s21_truncate(s21_decimal value, s21_decimal *result) {
    int value_scale = s21_get_scale(value.bits[3]);
    int value_sign = s21_get_bit(value.bits[3], 31);

    for (int i = 0; i < value_scale; i++) {
        s21_divide_by_10(&value);
    }

    *result = value;
    s21_set_bit(&result->bits[3], 31, value_sign);
    s21_set_scale(result, 0);
    return 0;
}

//  Возвращает результат умножения указанного Decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result) {
    int value_sign = s21_get_bit(value.bits[3], 31);
    *result = value;

    value_sign == 1 ? s21_set_bit(&result->bits[3], 31, 0) : s21_set_bit(&result->bits[3], 31, 1);

    return 0;
}
