#include "s21_decimal.h"

/**
 * Число 1 меньше числа 2 (number1 < number2)?
 * @return 0 - False, 1 - True
 */
int s21_is_less(s21_decimal number1, s21_decimal number2) {
    int is_greater_or_equal = s21_is_greater_or_equal(number1, number2);
    return is_greater_or_equal == 0 ? 1 : 0;
}

/**
 * Число 1 меньше либо равно числу 2 (number1 <= number2)?
 * @return 0 - False, 1 - True
 */
int s21_is_less_or_equal(s21_decimal number1, s21_decimal number2) {
    int is_greater = s21_is_greater(number1, number2);
    return is_greater == 0 ? 1 : 0;
}

/**
 * Число 1 больше числа 2 (number1 > number2)?
 * @return 0 - False, 1 - True
 */
int s21_is_greater(s21_decimal number1, s21_decimal number2) {
    int result = 0;
    int sign_value1 = s21_get_bit(number1.bits[3], 31);
    int sign_value2 = s21_get_bit(number2.bits[3], 31);
    int scale_value1 = s21_get_scale(number1.bits[3]);
    int scale_value2 = s21_get_scale(number2.bits[3]);

    // Если степени разные, то приводим их к одной
    if (scale_value1 != scale_value2) {
        s21_equalize_scale(&number1, &number2);
    }

    // Если знаки чисел равны, то сравниваем
    if (sign_value1 == sign_value2) {
        if (number1.bits[2] != number2.bits[2]) {
            if ((number1.bits[2] > number2.bits[2] && sign_value1 == 0) ||
                (number1.bits[2] < number2.bits[2] && sign_value1 == 1)) {
                result = 1;
            }
        } else if (number1.bits[1] != number2.bits[1]) {
            if ((number1.bits[1] > number2.bits[1] && sign_value1 == 0) ||
                (number1.bits[1] < number2.bits[1] && sign_value1 == 1)) {
                result = 1;
            }
        } else if (number1.bits[0] != number2.bits[1]) {
            if ((number1.bits[0] > number2.bits[0] && sign_value1 == 0) ||
                (number1.bits[0] < number2.bits[0] && sign_value1 == 1)) {
                result = 1;
            }
        }
    } else {
        if (!sign_value1) {
            result = 1;
        }
    }
    return result;
}

/**
 * Число 1 больше либо равно числу 2 (number1 >= number2)?
 * @return 0 - False, 1 - True
 */
int s21_is_greater_or_equal(s21_decimal number1, s21_decimal number2) {
    int is_greater = s21_is_greater(number1, number2);
    int is_equal = s21_is_equal(number1, number2);
    return is_equal || is_greater ? 1 : 0;
}

/**
 * Число 1 равно числу 2 (number1 == number2)?
 * @return 0 - False, 1 - True
 */
int s21_is_equal(s21_decimal number1, s21_decimal number2) {
    int number1_scale = s21_get_scale(number1.bits[3]);
    int number2_scale = s21_get_scale(number2.bits[3]);
    int value1_sign = s21_get_bit(number1.bits[3], 31);
    int value2_sign = s21_get_bit(number2.bits[3], 31);

    // Если степени разные, то приводим их к одной
    if (number1_scale != number2_scale) {
        s21_equalize_scale(&number1, &number2);
    }

    int is_sign_equal = value1_sign == value2_sign ? 1 : 0;
    int is_low_bits_equal = number1.bits[0] == number2.bits[0] ? 1 : 0;
    int is_mid_bits_equal = number1.bits[1] == number2.bits[1] ? 1 : 0;
    int is_high_bits_equal = number1.bits[2] == number2.bits[2] ? 1 : 0;

    return is_sign_equal && is_low_bits_equal && is_mid_bits_equal && is_high_bits_equal ? 1 : 0;
}

/**
 * Число 1 НЕ равно числу 2 (number1 != number2)?
 * @return 0 - False, 1 - True
 */
int s21_is_not_equal(s21_decimal number1, s21_decimal number2) {
    int is_equal = s21_is_equal(number1, number2);
    return is_equal == 0 ? 1 : 0;
}
