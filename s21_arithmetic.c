#include "s21_decimal.h"

/** Сложение двух чисел типа decmal.
 * @return:
 * 0 - ОК
 * 1 - число слишком велико или равно бесконечности
 * 2 - число слишком мало или равно отрицательной бесконечности */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error_code = 0;
    int sign_value1 = s21_get_bit(value_1.bits[3], 31);
    int sign_value2 = s21_get_bit(value_2.bits[3], 31);
    int scale_value1 = s21_get_scale(value_1.bits[3]);
    int scale_value2 = s21_get_scale(value_2.bits[3]);

    // Если степени разные, то приводим их к одной
    if (scale_value1 != scale_value2) {
        s21_equalize_scale(&value_1, &value_2);
    }

    // Если знаки чисел равны, то считаем их сумму
    if (sign_value1 == sign_value2) {
        int new_scale = s21_get_scale(value_1.bits[3]);

        // Сложение без учета степеней
        int add_result = s21_add_without_scale(value_1, value_2, result);
        s21_set_bit(&result->bits[3], 31, sign_value1);
        s21_set_scale(result, new_scale);

        // Если сложить не удалось, то делим оба числа на 10 и снова складываем
        if (add_result && new_scale != 0) {
            s21_divide_by_10(&value_1);
            s21_divide_by_10(&value_2);
            s21_set_scale(&value_1, new_scale - 1);
            s21_set_scale(&value_2, new_scale - 1);
            s21_add_without_scale(value_1, value_2, result);
            s21_set_bit(&result->bits[3], 31, sign_value1);
            s21_set_scale(result, new_scale - 1);
        } else if (add_result && sign_value1 == 0) {
            error_code = 1;
        } else if (add_result && sign_value1 == 1) {
            error_code = 2;
        }
    //  Если знаки чисел различны, то меняем знак у второго числа и вызываем функцию вычитания
    } else {
        sign_value2 ? s21_set_bit(&value_2.bits[3], 31, 0) : s21_set_bit(&value_2.bits[3], 31, 1);
        error_code = s21_sub(value_1, value_2, result);
    }
    return error_code;
}

/** Вычитание двух чисел типа decmal.
 * @return:
 * 0 - ОК
 * 1 - число слишком велико или равно бесконечности
 * 2 - число слишком мало или равно отрицательной бесконечности */
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
    int error_code = 0;
    int sign_value1 = s21_get_bit(value_1.bits[3], 31);
    int sign_value2 = s21_get_bit(value_2.bits[3], 31);
    int scale_value1 = s21_get_scale(value_1.bits[3]);
    int scale_value2 = s21_get_scale(value_2.bits[3]);

    // Если степени разные, то приводим их к одной
    if (scale_value1 != scale_value2) {
        s21_equalize_scale(&value_1, &value_2);
    }

    // Если знаки чисел равны, то меняем знак второго числа и считаем их сумму.
    // Иначе устанавливаем положительные знаки и применяем вычитание
    if (sign_value1 != sign_value2) {
        sign_value2 ? s21_set_bit(&value_2.bits[3], 31, 0) : s21_set_bit(&value_2.bits[3], 31, 1);
        error_code = s21_add(value_1, value_2, result);
    } else {
        s21_decimal *highest_number;
        s21_decimal *lowest_number;
        s21_set_bit(&value_1.bits[3], 31, 0);
        s21_set_bit(&value_2.bits[3], 31, 0);

        // Определяем наибольшее и наименьшее числа
        if (s21_is_greater(value_1, value_2)) {
            highest_number = &value_1;
            lowest_number = &value_2;
        } else {
            highest_number = &value_2;
            lowest_number = &value_1;
        }

        s21_set_bit(&value_1.bits[3], 31, sign_value1);
        s21_set_bit(&value_2.bits[3], 31, sign_value2);

        // Вычитание
        if (highest_number->bits[0] < lowest_number->bits[0]) {
            if (highest_number->bits[1]) {
                highest_number->bits[1]--;
                result->bits[0] = 4294967295 - lowest_number->bits[0] + highest_number->bits[0];
            } else {
                highest_number->bits[2]--;
                highest_number->bits[1] = 4294967294;
                result->bits[0] = 4294967295 - lowest_number->bits[0] + highest_number->bits[0];
            }
        } else {
            result->bits[0] = highest_number->bits[0] - lowest_number->bits[0];
        }
        if (highest_number->bits[1] < lowest_number->bits[1]) {
            if (highest_number->bits[2]) {
                highest_number->bits[2]--;
                result->bits[1] = 4294967295 - lowest_number->bits[1] + highest_number->bits[1];
            }
        } else {
            result->bits[1] = highest_number->bits[1] - lowest_number->bits[1];
        }
        result->bits[2] = highest_number->bits[2] - lowest_number->bits[2];
    }

    // Если первое число меньше второго, устанавливаем знак '-'
    if (s21_is_less(value_1, value_2)) {
        s21_set_bit(&result->bits[3], 31, 1);
    }
    int new_scale = s21_get_scale(value_1.bits[3]);
    s21_set_scale(result, new_scale);
    return error_code;
}

/** Умножение двух чисел типа decmal.
 * @return:
 * 0 - ОК
 * 1 - число слишком велико или равно бесконечности
 * 2 - число слишком мало или равно отрицательной бесконечности */
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error_code = 0;
    int sign_value1 = s21_get_bit(value_1.bits[3], 31);
    int sign_value2 = s21_get_bit(value_2.bits[3], 31);
    int scale_value1 = s21_get_scale(value_1.bits[3]);
    int scale_value2 = s21_get_scale(value_2.bits[3]);
    int sign_result = sign_value1 == sign_value2 ? 0 : 1;
    int scale_result = scale_value1 + scale_value2;
    int last_bit_position = 0;
    int last_bit_long_decimal_position = 0;
    s21_long_decimal res = {{0, 0, 0, 0, 0, 0, 0}};
    last_bit_position = s21_get_highest_bit(value_1);

    if (last_bit_position != 96) {
        for (int i = 0; i <= 95; i++) {
            s21_long_decimal temp_res = {{0, 0, 0, 0, 0, 0, 0}};
            int value1_bit = s21_get_bit(value_2.bits[i / 32], i % 32);
            if (value1_bit) {
                temp_res.bits[0] = value_1.bits[0];
                temp_res.bits[1] = value_1.bits[1];
                temp_res.bits[2] = value_1.bits[2];
                s21_shift_left_long_decimal(&temp_res, i);
                s21_bit_addition(&res, &temp_res);
            }
        }
        last_bit_long_decimal_position = s21_get_last_bit_long_decimal(res);
        while ((last_bit_long_decimal_position > 95 && scale_result != 0) ||
                (last_bit_long_decimal_position < 96 && scale_result > 28)) {
            s21_divide_by_10_long_decimal(&res);
            last_bit_long_decimal_position = s21_get_last_bit_long_decimal(res);
            scale_result--;
        }
        if (last_bit_long_decimal_position > 95 && sign_result) {
            error_code = 2;
        } else if (last_bit_long_decimal_position > 95 && !sign_result) {
            error_code = 1;
        } else {
            result->bits[0] = res.bits[0];
            result->bits[1] = res.bits[1];
            result->bits[2] = res.bits[2];
            s21_set_scale(result, scale_result);
            s21_set_bit(&result->bits[3], 31, sign_result);
        }
    }
    return error_code;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error_code = 0;
    int value1_sign = s21_get_bit(value_1.bits[3], 31);
    int value2_sign = s21_get_bit(value_2.bits[3], 31);
    int value1_scale = s21_get_scale(value_1.bits[3]);
    int value2_scale = s21_get_scale(value_2.bits[3]);
    int sign_result = value1_sign == value2_sign ? 0 : 1;
    int dif_scale = value1_scale > value2_scale ? value1_scale - value2_scale : value2_scale - value1_scale;
    s21_long_decimal value1_long = {{0, 0, 0, 0, 0, 0, 0}};
    int last_bit_long_decimal_position = 0;
    int count = 0;
    int minus_dif = 0;
    s21_decimal remainder = {{0, 0, 0, 0}};

    if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
        error_code = 3;
        result->bits[0] = 0;
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = 0;
    } else if (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0) {
        result->bits[0] = 0;
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = 0;
    } else if (value_1.bits[0] == value_2.bits[0] && value_1.bits[1] == value_2.bits[1] &&
        value_1.bits[2] == value_2.bits[2]) {
        result->bits[0] = 1;
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = 0;
    } else {
        value1_long.bits[0] = value_1.bits[0];
        value1_long.bits[1] = value_1.bits[1];
        value1_long.bits[2] = value_1.bits[2];

        if (value1_scale > value2_scale) {
            for (int i = 0; i < dif_scale; i++) {
                s21_divide_by_10_long_decimal(&value1_long);
            }
        } else {
            for (int i = 0; i < dif_scale; i++) {
                s21_multiply_long_decimal_by_10(&value1_long);
                s21_set_scale(&value_2, value2_scale - i - 1);
                minus_dif++;
            }
            dif_scale -= minus_dif;
            s21_set_bit(&value_2.bits[3], 31, 0);
        }
        remainder = s21_divide_long_decimal_by_decimal(&value1_long, value_2);

        last_bit_long_decimal_position = s21_get_last_bit_long_decimal(value1_long);
        while (last_bit_long_decimal_position > 95 && dif_scale != 0) {
            s21_divide_by_10_long_decimal(&value1_long);
            last_bit_long_decimal_position = s21_get_last_bit_long_decimal(value1_long);
            dif_scale--;
            count++;
        }
        if ((last_bit_long_decimal_position > 95 && sign_result) ||
        (s21_get_scale(remainder.bits[3]) == 30)) {
            error_code = 2;
        } else if (last_bit_long_decimal_position > 95 && !sign_result) {
            error_code = 1;
        } else {
            result->bits[0] = value1_long.bits[0];
            result->bits[1] = value1_long.bits[1];
            result->bits[2] = value1_long.bits[2];
            for (int i = 0; i < dif_scale; i++) {
                s21_divide_by_10(result);
            }

            if (s21_get_scale(remainder.bits[3]) < 29) {
                s21_set_scale(result, s21_get_scale(remainder.bits[3]));
            }

            s21_set_bit(&result->bits[3], 31, sign_result);
        }
    }
    return error_code;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int error_code = 0;
    int value1_scale = s21_get_scale(value_1.bits[3]);
    int value2_scale = s21_get_scale(value_2.bits[3]);
    int value1_sign = s21_get_bit(value_1.bits[3], 31);
    int dif_scale = value1_scale > value2_scale ? value1_scale - value2_scale : value2_scale - value1_scale;
    s21_long_decimal value1_long = {{0, 0, 0, 0, 0, 0, 0}};

    s21_set_bit(&value_1.bits[3], 31, 0);
    s21_set_bit(&value_2.bits[3], 31, 0);
    if (s21_is_greater_or_equal(value_1, value_2)) {
        if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
            error_code = 3;
        } else {
            value1_long.bits[0] = value_1.bits[0];
            value1_long.bits[1] = value_1.bits[1];
            value1_long.bits[2] = value_1.bits[2];

            if (value1_scale > value2_scale) {
                for (int i = 0; i < dif_scale; i++) {
                    s21_divide_by_10_long_decimal(&value1_long);
                    dif_scale--;
                }
            } else {
                for (int i = 0; i < dif_scale; i++) {
                    s21_multiply_long_decimal_by_10(&value1_long);
                    s21_set_scale(&value_2, value2_scale - 1);
                }
            }
            value_2.bits[3] = 300;
            *result = s21_divide_long_decimal_by_decimal(&value1_long, value_2);
            for (int i = 0; i < dif_scale; i++) {
                s21_divide_by_10(result);
            }
            value1_sign ? s21_set_bit(&result->bits[3], 31, 1) : s21_set_bit(&result->bits[3], 31, 0);
        }
    } else {
        result->bits[0] = value_1.bits[0];
        result->bits[1] = value_1.bits[1];
        result->bits[2] = value_1.bits[2];
        s21_set_scale(result, dif_scale);
    }
    return error_code;
}
