#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <math.h>
#define MINIMUM(x, y) x < y ? x : y;
#define MAXIMUM(x, y) x > y ? x : y;
#define s21_NAN 0.0 / 0.0
#define s21_INF 1.0 / 0.0
#define s21_NEGATIVE_INF -1.0 / 0.0

typedef struct {
    unsigned int bits[4];
} s21_decimal;

typedef struct {
    unsigned int bits[7];
} s21_long_decimal;

typedef union {
    int integer_part;
    float float_part;
} floatbits;

// arithmetic functions
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// comparison functions
int s21_is_less(s21_decimal number1, s21_decimal number2);
int s21_is_less_or_equal(s21_decimal number1, s21_decimal number2);
int s21_is_greater(s21_decimal number1, s21_decimal number2);
int s21_is_greater_or_equal(s21_decimal number1, s21_decimal number2);
int s21_is_equal(s21_decimal number1, s21_decimal number2);
int s21_is_not_equal(s21_decimal number1, s21_decimal number2);

// converters functions
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// another functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// help functions
int s21_get_bit(unsigned int number, int index);
int s21_get_scale(unsigned int number);
void s21_equalize_scale(s21_decimal *value_1, s21_decimal *value_2);
int s21_multiplication_by_10_is_possible(s21_decimal number);
void s21_shift_left(s21_decimal *number, int number_of_shifts);
int s21_add_without_scale(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_multiply_by_10(s21_decimal *number);
void s21_set_scale(s21_decimal *number, int scale);
void s21_set_bit(unsigned int *number, int index, int bit);
unsigned int s21_divide_by_10(s21_decimal *number);
unsigned int s21_set_sign(int number, s21_decimal *dec);
int s21_get_float_sign(float *src);
int s21_get_float_exp(float *src);
int s21_get_last_bit(unsigned int number);
void s21_bit_addition(s21_long_decimal *var1, s21_long_decimal *var2);
void s21_shift_left_long_decimal(s21_long_decimal *number, int number_of_shifts);
void s21_divide_by_10_long_decimal(s21_long_decimal *number);
int s21_get_last_bit_long_decimal(s21_long_decimal number);
int s21_get_highest_bit(s21_decimal number);
int s21_get_bit_new(s21_decimal number, int bit_position);
int s21_get_bit_new_long_decimal(s21_long_decimal number, int bit_position);

void s21_multiply_long_decimal_by_10(s21_long_decimal *number);
void s21_add_without_scale_for_long_decimal(s21_long_decimal value_1,
                                            s21_long_decimal value_2, s21_long_decimal *result);
s21_decimal s21_divide_long_decimal_by_decimal(s21_long_decimal *number1, s21_decimal number2);
void s21_shift_right(s21_decimal *number);
void s21_set_zero_for_result(s21_decimal *n);
void s21_set_inf(s21_decimal *val);

#endif  // SRC_S21_DECIMAL_H_
