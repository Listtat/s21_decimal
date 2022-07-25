#include "s21_decimal.h"

// Получение бита по известному индексу
int s21_get_bit(unsigned int number, int index) {
    return (number & (1 << index)) != 0; }

// Получение степени числа
int s21_get_scale(unsigned int number) {
    int scale = 0;
    number <<= 1;
    number >>= 17;
    scale = (int) number;
    return scale;
}

// Приведение двух чисел к одной степени
void s21_equalize_scale(s21_decimal *value_1, s21_decimal *value_2) {
    int scale_value1 = s21_get_scale(value_1->bits[3]);
    int scale_value2 = s21_get_scale(value_2->bits[3]);
    unsigned int lowest_scale = MINIMUM(scale_value1, scale_value2);
    unsigned int highest_scale = MAXIMUM(scale_value1, scale_value2);
    unsigned int dif_scale = highest_scale - lowest_scale;
    s21_decimal *lowest_scale_number;
    s21_decimal *highest_scale_number;

    //  Находим число с наименьшей и наибольшей степенями
    if (scale_value1 > scale_value2) {
        highest_scale_number = value_1;
        lowest_scale_number = value_2;
    } else {
        highest_scale_number = value_2;
        lowest_scale_number = value_1;
    }

    /* Пока не уравняем степени, умножаем число с наименьшей степенью на 10.
     * Если умножать уже нельзя, то делим число с наибольшей степеню на 10 (теряется точность). */
    for (unsigned int i = 0; i < dif_scale; i++) {
        if (s21_multiplication_by_10_is_possible(*lowest_scale_number) &&
        (s21_get_scale(lowest_scale_number->bits[3]) < 28)) {
            int current_scale = s21_get_scale(lowest_scale_number->bits[3]);
            s21_multiply_by_10(lowest_scale_number);
            s21_set_scale(lowest_scale_number, current_scale + 1);
        } else {
            int current_scale = s21_get_scale(highest_scale_number->bits[3]);
            if (current_scale > 0) {
                s21_divide_by_10(highest_scale_number);
                s21_set_scale(highest_scale_number, current_scale - 1);
            }
        }
    }
}

/** Проверка, возможно ли умножение на 10
 0 - Невозможно, 1 - возможно */
int s21_multiplication_by_10_is_possible(s21_decimal number) {
    int possible = 0;
    s21_decimal result = {{0, 0, 0, 0}};
    int bit_31 = s21_get_bit(number.bits[2], 31);
    int bit_30 = s21_get_bit(number.bits[2], 30);
    int bit_29 = s21_get_bit(number.bits[2], 29);

    /*  Если 3 последних бита равны 0, то выполняем сложение без учета степеней чисел:
     * number * 8 + number * 2. Если сложение удалось, то умножение на 10 возможно */
    if (bit_31 == 0 && bit_30 == 0 && bit_29 == 0) {
        s21_decimal number_multiplied_by_8 = number;
        s21_shift_left(&number_multiplied_by_8, 3);
        s21_decimal number_multiplied_by_2 = number;
        s21_shift_left(&number_multiplied_by_2, 1);
        if (s21_add_without_scale(number_multiplied_by_8, number_multiplied_by_2, &result) == 0) {
            possible = 1;
        }
    }
    return possible;
}

// Сдвиг числа влево на заданное количество битов
void s21_shift_left(s21_decimal *number, int number_of_shifts) {
    for (int i = 0; i < number_of_shifts; i++) {
        int low_last_bit = s21_get_bit(number->bits[0], 31);
        int mid_last_bit = s21_get_bit(number->bits[1], 31);

        number->bits[0] <<= 1;
        number->bits[1] <<= 1;
        number->bits[2] <<= 1;

        s21_set_bit(&number->bits[1], 0, low_last_bit);
        s21_set_bit(&number->bits[2], 0, mid_last_bit);
    }
}

// Cложение без учета степеней. 0 - Успешно, 1 - Переполнение
int s21_add_without_scale(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    result->bits[0] = result->bits[1] = result->bits[2] = 0;
    int error_oversize = 0;
    unsigned int memory_MID = 0;
    unsigned int memory_HIGH = 0;

    // Проверка на переполнение младших битов и сложение
    unsigned int lowest_number = MINIMUM(value_1.bits[0], value_2.bits[0]);
    if (value_1.bits[0] + value_2.bits[0] < lowest_number) {
        result->bits[1]++;
        memory_MID++;
    }
    result->bits[0] = value_1.bits[0] + value_2.bits[0];

    // Проверка на переполнение средних битов и сложение
    lowest_number = MINIMUM(value_1.bits[1], value_2.bits[1]);
    if ((value_1.bits[1] + value_2.bits[1] + result->bits[1] < lowest_number) ||
    (memory_MID && (value_1.bits[1] == 0 || value_2.bits[1] == 0))) {
        result->bits[2]++;
        memory_HIGH++;
    }
    result->bits[1] = value_1.bits[1] + value_2.bits[1] + result->bits[1];

    // Проверка на переполнение старших битов и сложение
    lowest_number = MINIMUM(value_1.bits[2], value_2.bits[2]);
    if ((value_1.bits[2] + value_2.bits[2] + result->bits[2] < lowest_number) ||
        (memory_HIGH && (value_1.bits[2] == 4294967295 || value_2.bits[2] == 4294967295))) {
        result->bits[0] = 0;
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = 0;
        error_oversize = 1;
    } else {
        result->bits[2] = value_1.bits[2] + value_2.bits[2] + result->bits[2];
    }
    return error_oversize;
}

// Умножение числа на 10
void s21_multiply_by_10(s21_decimal *number) {
    s21_decimal number_multiplied_by_8 = *number;
    s21_shift_left(&number_multiplied_by_8, 3);
    s21_decimal number_multiplied_by_2 = *number;
    s21_shift_left(&number_multiplied_by_2, 1);
    s21_add_without_scale(number_multiplied_by_8, number_multiplied_by_2, number);
}

// Взятие степени числа
void s21_set_scale(s21_decimal *number, int scale) {
    int sign = s21_get_bit(number->bits[3], 31);
    number->bits[3] = scale;
    number->bits[3] <<= 16;
    if (sign == 1) {
        s21_set_bit(&number->bits[3], 31, sign);
    }
}

// Установка нужного бита в нужное место
void s21_set_bit(unsigned int *number, int index, int bit) {
    bit == 1 ? (*number |= (1 << index)) : (*number &= ~(1 << index));
}

/** Деление числа на 10
 * @return остаток
 */
unsigned int s21_divide_by_10(s21_decimal *number) {
    unsigned int buffer = 0;
    int last_bit_position = 0;
    int scale_of_number = s21_get_scale(number->bits[3]);
    s21_decimal result = {{0, 0, 0, 0}};

    last_bit_position = s21_get_highest_bit(*number);

    for (int i = 0; i < last_bit_position + 2; i++) {
        if (buffer >= 10) {
            // Записываем 1 в результат
            s21_shift_left(&result, 1);
            s21_set_bit(&result.bits[0], 0, 1);

            // Вычитаем 1010 из накопленных битов
            buffer = buffer - 10u;

            // Берём ещё бит
            int new_bit = s21_get_bit_new(*number, last_bit_position - i);
            buffer = buffer << 1;
            s21_set_bit(&buffer, 0, new_bit);
        } else {
            // Записываем 0 в результат
            s21_shift_left(&result, 1);
            s21_set_bit(&result.bits[0], 0, 0);

            // Берём ещё бит
            int new_bit = s21_get_bit_new(*number, last_bit_position - i);
            buffer = buffer << 1;
            s21_set_bit(&buffer, 0, new_bit);
        }
    }

    buffer = buffer >> 1;
    *number = result;
    s21_set_scale(number, scale_of_number);

    return buffer;
}

// Установка знака
unsigned int s21_set_sign(int number, s21_decimal *dec) {
    unsigned int new_number;
    int sign = 0;
    if (number < 0) {
        if (number == -2147483648) {
            new_number = 2147483648;
        }
        number *= -1;
        sign = 1;
        new_number = number;
    } else {
        new_number = number;
    }
    s21_set_bit(&dec->bits[3], 31, sign);
    return new_number;
}

// Взятие знака у числа float
int s21_get_float_sign(float *src) { return *(int *)src >> 31; }

// Взятие степени у числа float
int s21_get_float_exp(float *src) { return ((*(int *)src & ~0x80000000) >> 23) - 127;}

// Нахождение крайнего "включенного" бита. Аргумент - определенный байт массива decimal
int s21_get_last_bit(unsigned int number) {
    int last_bit = 31;
    for (; last_bit >= 0 && s21_get_bit(number, last_bit) == 0 ; last_bit--) {}
    return last_bit;
}

//  Побитовое сложение
void s21_bit_addition(s21_long_decimal *var1, s21_long_decimal *var2) {
    int buffer = 0;

    for (int i = 0; i < 224; i++) {
        int cur_bit_of_var1 = s21_get_bit(var1->bits[i / 32], i % 32);
        int cur_bit_of_var2 = s21_get_bit(var2->bits[i / 32], i % 32);

        if (!cur_bit_of_var1 && !cur_bit_of_var2) {  // оба бита выключены
            if (buffer) {
                s21_set_bit(&var1->bits[i / 32], i % 32, 1);
                buffer = 0;
            } else {
                s21_set_bit(&var1->bits[i / 32], i % 32, 0);
            }
        } else if (cur_bit_of_var1 != cur_bit_of_var2) {
            if (buffer) {
                s21_set_bit(&var1->bits[i / 32], i % 32, 0);
                buffer = 1;
            } else {
                s21_set_bit(&var1->bits[i / 32], i % 32, 1);
            }
        } else {  // оба бита включены
            if (buffer) {
                s21_set_bit(&var1->bits[i / 32], i % 32, 1);
                buffer = 1;
            } else {
                s21_set_bit(&var1->bits[i / 32], i % 32, 0);
                buffer = 1;
            }
        }
    }
}

//  Сдвиг числа long_deciml влево на заданное количество битов
void s21_shift_left_long_decimal(s21_long_decimal *number, int number_of_shifts) {
    for (int i = 0; i < number_of_shifts; i++) {
        int last_bit_0 = s21_get_bit(number->bits[0], 31);
        int last_bit_1 = s21_get_bit(number->bits[1], 31);
        int last_bit_2 = s21_get_bit(number->bits[2], 31);
        int last_bit_3 = s21_get_bit(number->bits[3], 31);
        int last_bit_4 = s21_get_bit(number->bits[4], 31);
        int last_bit_5 = s21_get_bit(number->bits[5], 31);

        number->bits[0] <<= 1;
        number->bits[1] <<= 1;
        number->bits[2] <<= 1;
        number->bits[3] <<= 1;
        number->bits[4] <<= 1;
        number->bits[5] <<= 1;
        number->bits[6] <<= 1;

        s21_set_bit(&number->bits[1], 0, last_bit_0);
        s21_set_bit(&number->bits[2], 0, last_bit_1);
        s21_set_bit(&number->bits[3], 0, last_bit_2);
        s21_set_bit(&number->bits[4], 0, last_bit_3);
        s21_set_bit(&number->bits[5], 0, last_bit_4);
        s21_set_bit(&number->bits[6], 0, last_bit_5);
    }
}

// Деление числа на 10 для long_decimal
void s21_divide_by_10_long_decimal(s21_long_decimal *number) {
    unsigned int buffer = 0;
    int last_bit_position = 0;
    unsigned int memory_round = 0;
    s21_long_decimal result = {{0, 0, 0, 0, 0, 0, 0}};

    last_bit_position = s21_get_last_bit_long_decimal(*number);

    if (number->bits[0] % 10 < 5) {
        memory_round = 0;
    } else {
        memory_round = 1;
    }

    for (int i = 0; i < last_bit_position + 2; i++) {
        if (buffer >= 10) {
            // Записываем 1 в результат
            s21_shift_left_long_decimal(&result, 1);
            s21_set_bit(&result.bits[0], 0, 1);

            // Вычитаем 1010 из накопленных битов
            buffer = buffer - 10u;

            // Берём ещё бит
            int new_bit = s21_get_bit_new_long_decimal(*number, last_bit_position - i);
            buffer = buffer << 1;
            s21_set_bit(&buffer, 0, new_bit);
        } else {
            // Записываем 0 в результат
            s21_shift_left_long_decimal(&result, 1);
            s21_set_bit(&result.bits[0], 0, 0);

            // Берём ещё бит
            int new_bit = s21_get_bit_new_long_decimal(*number, last_bit_position - i);
            buffer = buffer << 1;
            s21_set_bit(&buffer, 0, new_bit);
        }
    }

    result.bits[0] += memory_round ? 1 : 0;
    buffer = buffer >> 1;
    *number = result;

    printf(" ");
}

// Нахождение крайнего "включенного" бита для числа long_decimal
int s21_get_last_bit_long_decimal(s21_long_decimal number) {
    int last_bit_long_decimal_position = -1;

    for (int i = 1; i < 225 && last_bit_long_decimal_position == -1; i++) {
        if (s21_get_bit_new_long_decimal(number, (224 - i)) == 1) {
            last_bit_long_decimal_position = (224 - i);
        }
    }
    return last_bit_long_decimal_position;
}

// Нахождение крайнего "включенного" бита. Аргумент - decimal
int s21_get_highest_bit(s21_decimal number) {
    int bit_position = -1;

    for (int i = 0; i < 96 && bit_position == -1; i++) {
        if (s21_get_bit_new(number, (95 - i)) == 1) {
            bit_position = (95 - i);
        }
    }

    return bit_position;
}

// Нахождения бита при передачи в аргументы числа decimal
int s21_get_bit_new(s21_decimal number, int bit_position) {
    int bit_number = bit_position / 32;
    bit_position = bit_position % 32;
    int bit = (int) ((number.bits[bit_number] >> bit_position) & 1u);

    return bit;
}

// Нахождения бита при передачи в аргументы числа long_decimal
int s21_get_bit_new_long_decimal(s21_long_decimal number, int bit_position) {
    int bit_number = bit_position / 32;
    bit_position = bit_position % 32;
    int bit = (int) ((number.bits[bit_number] >> bit_position) & 1u);

    return bit;
}

//  Умножение числа long_decimal на 10
void s21_multiply_long_decimal_by_10(s21_long_decimal *number) {
    s21_long_decimal number_multiplied_by_8 = *number;
    s21_shift_left_long_decimal(&number_multiplied_by_8, 3);
    s21_long_decimal number_multiplied_by_2 = *number;
    s21_shift_left_long_decimal(&number_multiplied_by_2, 1);
    s21_add_without_scale_for_long_decimal(number_multiplied_by_8, number_multiplied_by_2, number);
}

// Cложение без учета степеней чисел long_decimal. 0 - Успешно, 1 - Переполнение
void s21_add_without_scale_for_long_decimal(s21_long_decimal value_1,
                                            s21_long_decimal value_2, s21_long_decimal *result) {
    result->bits[0] = result->bits[1] = result->bits[2] =
            result->bits[3] = result->bits[4] = result->bits[5] = result->bits[6] = 0;
    unsigned int memory_1_bit = 0;
    unsigned int memory_2_bit = 0;
    unsigned int memory_3_bit = 0;
    unsigned int memory_4_bit = 0;
    unsigned int memory_5_bit = 0;

    // Проверка на переполнение нулевых битов и сложение
    unsigned int lowest_number = MINIMUM(value_1.bits[0], value_2.bits[0]);
    if (value_1.bits[0] + value_2.bits[0] < lowest_number) {
        result->bits[1]++;
        memory_1_bit++;
    }
    result->bits[0] = value_1.bits[0] + value_2.bits[0];

    // Проверка на переполнение первых битов и сложение
    lowest_number = MINIMUM(value_1.bits[1], value_2.bits[1]);
    if ((value_1.bits[1] + value_2.bits[1] + result->bits[1] < lowest_number) ||
        (memory_1_bit && (value_1.bits[1] == 4294967295 || value_2.bits[1] == 4294967295))) {
        result->bits[2]++;
        memory_2_bit++;
    }
    result->bits[1] = value_1.bits[1] + value_2.bits[1] + result->bits[1];

    // Проверка на переполнение вторых битов и сложение
    lowest_number = MINIMUM(value_1.bits[2], value_2.bits[2]);
    if ((value_1.bits[2] + value_2.bits[2] + result->bits[2] < lowest_number) ||
        (memory_2_bit && (value_1.bits[2] == 4294967295 || value_2.bits[2] == 4294967295))) {
        result->bits[3]++;
        memory_3_bit++;
    }
    result->bits[2] = value_1.bits[2] + value_2.bits[2] + result->bits[2];

    // Проверка на переполнение третьих битов и сложение
    lowest_number = MINIMUM(value_1.bits[3], value_2.bits[3]);
    if ((value_1.bits[3] + value_2.bits[3] + result->bits[3] < lowest_number) ||
        (memory_3_bit && (value_1.bits[3] == 4294967295 || value_2.bits[3] == 4294967295))) {
        result->bits[4]++;
        memory_4_bit++;
    }
    result->bits[3] = value_1.bits[3] + value_2.bits[3] + result->bits[3];

    // Проверка на переполнение четвертых битов битов и сложение
    lowest_number = MINIMUM(value_1.bits[4], value_2.bits[4]);
    if ((value_1.bits[4] + value_2.bits[4] + result->bits[4] < lowest_number) ||
        (memory_4_bit && (value_1.bits[4] == 4294967295 || value_2.bits[4] == 4294967295))) {
        result->bits[5]++;
        memory_5_bit++;
    }
    result->bits[4] = value_1.bits[4] + value_2.bits[4] + result->bits[4];

    // Проверка на переполнение пятых битов битов и сложение
    lowest_number = MINIMUM(value_1.bits[5], value_2.bits[5]);
    if ((value_1.bits[5] + value_2.bits[5] + result->bits[5] < lowest_number) ||
        (memory_5_bit && (value_1.bits[5] == 4294967295 || value_2.bits[5] == 4294967295))) {
        result->bits[6]++;
    }
    result->bits[5] = value_1.bits[5] + value_2.bits[5] + result->bits[5];
    result->bits[6] = value_1.bits[6] + value_2.bits[6] + result->bits[6];
}

// Деление чисел long_decimal
s21_decimal s21_divide_long_decimal_by_decimal(s21_long_decimal *number1, s21_decimal number2) {
    unsigned int check_mod = number2.bits[3];
    number2.bits[3] = 0;
    s21_decimal buffer = {{0,  0,  0 , 0}};
    s21_decimal temp = {{0,  0,  0, 0}};
    int last_bit_position = 0;
    int last_bit_of_remainder = 0;
    int scale = 0;
    int flag = 0;
    s21_long_decimal result = {{0, 0, 0, 0, 0, 0, 0}};

    last_bit_position = s21_get_last_bit_long_decimal(*number1);

    for (int i = 0; i < last_bit_position + 2; i++) {
        if (s21_is_greater_or_equal(buffer, number2)) {
            // Записываем 1 в результат
            s21_shift_left_long_decimal(&result, 1);
            s21_set_bit(&result.bits[0], 0, 1);

            // Вычитаем number2 из накопленных битов
            temp = buffer;
            s21_sub(temp, number2, &buffer);

            // Берём ещё бит
            int new_bit = s21_get_bit_new_long_decimal(*number1, last_bit_position - i);
            s21_shift_left(&buffer, 1);
            s21_set_bit(&buffer.bits[0], 0, new_bit);
        } else {
            // Записываем 0 в результат
            s21_shift_left_long_decimal(&result, 1);
            s21_set_bit(&result.bits[0], 0, 0);

            // Берём ещё бит
            int new_bit = s21_get_bit_new_long_decimal(*number1, last_bit_position - i);
            s21_shift_left(&buffer, 1);
            s21_set_bit(&buffer.bits[0], 0, new_bit);
        }
    }

    s21_shift_right(&buffer);
    s21_decimal buffer_1 = buffer;

    last_bit_position = s21_get_last_bit_long_decimal(result);
    if ((check_mod != 300) && (buffer.bits[0] != 0 || buffer.bits[1] != 0 || buffer.bits[2] != 0)) {
        while (last_bit_position < 96 && scale < 29) {
            if (buffer.bits[0] == 0 && buffer.bits[1] == 0 && buffer.bits[2] == 0) {
                buffer = buffer_1;
                s21_set_scale(&buffer, 30);
                flag = 1;
                break;
            }
            unsigned int count = 0;

            last_bit_of_remainder = s21_get_highest_bit(buffer);
            if (s21_is_less(buffer, number2)) {
                s21_multiply_by_10(&buffer);
                last_bit_of_remainder = s21_get_highest_bit(buffer);
                scale++;
            }
            for (int i = 0; i < last_bit_of_remainder + 2; i++) {
                if (s21_is_greater_or_equal(buffer, number2)) {
                    count++;
                    temp = buffer;
                    s21_sub(temp, number2, &buffer);

                } else {
                    s21_multiply_long_decimal_by_10(&result);
                    s21_long_decimal buf_result = result;
                    s21_long_decimal buf_count = {{count, 0, 0, 0, 0, 0, 0}};
                    s21_add_without_scale_for_long_decimal(buf_result, buf_count, &result);
                    break;
                }
            }
            last_bit_position = s21_get_last_bit_long_decimal(result);
        }
    }

    if (flag || (!flag && (last_bit_position > 95))) {
        s21_divide_by_10_long_decimal(&result);
        scale--;
    }

    *number1 = result;
    if (s21_get_scale(buffer.bits[3]) < 29 && !flag) {
        s21_set_scale(&buffer, scale);
    }
    return buffer;
}

/**
 * Самописный сдвиг вправо (96 бит)
 */
void s21_shift_right(s21_decimal *number) {
    int mid_first_bit = s21_get_bit_new(*number, 32);
    int high_first_bit = s21_get_bit_new(*number, 64);

    number->bits[0] >>= 1;
    number->bits[1] >>= 1;
    number->bits[2] >>= 1;

    s21_set_bit(&number->bits[1], 31, high_first_bit);
    s21_set_bit(&number->bits[0], 31, mid_first_bit);
}

// Зануление числа decimal
void s21_set_zero_for_result(s21_decimal *n) {
    for (int i = 0; i < 4; i++) {
        n->bits[i] = 0;
    }
}

// Что-то странное. Проверить
void s21_set_inf(s21_decimal *val) {
    for (int i = 0; i < 4; i++) {
        val->bits[i] = 0;
    }
    s21_set_scale(val, 28);
}
