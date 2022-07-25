
#include "s21_decimal.h"
#include <check.h>

#define NaN 0.0 / 0.0
#define inf 1.0 / 0.0
#define neg_inf -1.0 / 0.0

s21_decimal zero = {{0, 0, 0, 0}};
s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
s21_decimal negative_value1 = {{150, 0, 0, 0x80000000}};
s21_decimal negative_value2 = {{125, 0, 0, 0x80000000}};

s21_decimal result1 = {{0, 0, 0, 0}};
s21_decimal result2 = {{0, 0, 0, 0}};
s21_decimal case_full1 = {{123, 321, -2147483648, 0}};
s21_decimal case_full2 = {{123, 321, -2147483648, 0}};
s21_decimal case1 = {{0, 0, 0, 0}};
s21_decimal case2 = {{1, 0, 0, 0}};
s21_decimal case3 = {{0x7FFFFFFF, 0, 0, 0}};
s21_decimal case12 = {{150, 0, 0, 0x80000000}};
s21_decimal case13 = {{2, 0, 0, 0x80000000}};
s21_decimal case7 = {{75, 0, 0, 0}};

START_TEST(is_less_test) {
    int result = s21_is_less(zero, negative_value1);
    int origin = (0 < -150);
    ck_assert_int_eq(result, origin);

    result = s21_is_less(zero, zero);
    origin = 0;
    ck_assert_int_eq(result, origin);

    result = s21_is_less(case_max, case_min);
    origin = 0;
    ck_assert_int_eq(result, origin);

    result = s21_is_less(negative_value1, negative_value2);
    origin = 1;
    ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(is_less_or_equal_test) {
    int result = s21_is_less_or_equal(zero, negative_value1);
    int origin = 0;
    ck_assert_int_eq(result, origin);

    result = s21_is_less_or_equal(case_max, case_max);
    origin = 1;
    ck_assert_int_eq(result, origin);

    result = s21_is_less_or_equal(negative_value1, negative_value2);
    origin = 1;
    ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(is_greater_test) {
    int result = s21_is_greater(case_max, case_min);
    int origin = (2147483647 > -2147483648);
    ck_assert_int_eq(result, origin);

    float fl1 = 0.005, fl2 = 0.0049;
    s21_decimal float_dec1, float_dec2;
    s21_from_float_to_decimal(fl1, &float_dec1);
    s21_from_float_to_decimal(fl2, &float_dec2);
    result = s21_is_greater(float_dec1, float_dec2);
    ck_assert_int_eq(result, fl1 > fl2);

    fl1 = 1e-10, fl2 = 1e-12;
    s21_from_float_to_decimal(fl1, &float_dec1);
    s21_from_float_to_decimal(fl2, &float_dec2);
    result = s21_is_greater(float_dec1, float_dec2);
    ck_assert_int_eq(result, fl1 > fl2);
}
END_TEST

START_TEST(is_greater_or_equal_test) {
    int result = s21_is_greater_or_equal(zero, case_min);
    int origin = (0 >= -2147483648);
    ck_assert_int_eq(result, origin);

    float fl1 = 0.005, fl2 = -0.0049;
    s21_decimal float_dec1, float_dec2;
    s21_from_float_to_decimal(fl1, &float_dec1);
    s21_from_float_to_decimal(fl2, &float_dec2);
    result = s21_is_greater_or_equal(float_dec1, float_dec2);
    ck_assert_int_eq(result, 1);

    result = s21_is_greater_or_equal(float_dec1, float_dec1);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(is_equal_test) {
    int result = s21_is_equal(case_min, case_min);
    int origin = 1;
    ck_assert_int_eq(result, origin);

    result = s21_is_equal(negative_value1, negative_value2);
    origin = 0;
    ck_assert_int_eq(result, origin);

    float fl1 = -0.0005, fl2 = -0.00049;
    s21_decimal fl1_d, fl2_d;
    s21_from_float_to_decimal(fl1, &fl1_d);
    s21_from_float_to_decimal(fl2, &fl2_d);
    result = s21_is_equal(fl1_d, fl2_d);
    origin = (-0.0005 == -0.00049);
    ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(is_not_equal_test) {
    s21_decimal d1 = {{0, 1, 0, 0}};
    s21_decimal d2 = {{0, 0, 1, 0}};
    int result = s21_is_not_equal(d1, d2);
    int origin = 1;
    ck_assert_int_eq(result, origin);

    result = s21_is_not_equal(case_max, case_min);
    origin = 1;
    ck_assert_int_eq(result, origin);

    result = s21_is_not_equal(zero, zero);
    origin = 0;
    ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_round_test) {
    float a = 0.4;
    s21_decimal fl_d;
    s21_from_float_to_decimal(a, &fl_d);
    s21_round(fl_d, &fl_d);
    float result;
    s21_from_decimal_to_float(fl_d, &result);
    ck_assert_double_eq(result, 0.0);

    a = -0.5;
    s21_from_float_to_decimal(a, &fl_d);
    s21_round(fl_d, &fl_d);
    s21_from_decimal_to_float(fl_d, &result);
    ck_assert_double_eq(result, -1.0);
}
END_TEST

START_TEST(s21_floor_test) {
    float a = 0.4;
    s21_decimal fl_d;
    s21_from_float_to_decimal(a, &fl_d);
    s21_floor(fl_d, &fl_d);
    float result;
    s21_from_decimal_to_float(fl_d, &result);
    ck_assert_double_eq(result, 0.0);

    a = -0.4;
    s21_from_float_to_decimal(a, &fl_d);
    s21_floor(fl_d, &fl_d);
    s21_from_decimal_to_float(fl_d, &result);
    ck_assert_double_eq(result, -1.0);
    a = 0;
    s21_from_float_to_decimal(a, &fl_d);
    s21_floor(fl_d, &fl_d);
    s21_from_decimal_to_float(fl_d, &result);
    ck_assert_double_eq(result, 0.0);
}
END_TEST

START_TEST(int_converters_test) {
    s21_decimal dec, big_dec = {{23536, 37375, 5647, 0}};
    int zero_i = 0, n1 = -1233456789, n2 = 98765321;
    int zero_res, res1, res2;
    int r1 = s21_from_int_to_decimal(zero_i, &dec);
    int r2 = s21_from_decimal_to_int(dec, &zero_res);
    ck_assert_int_eq(zero_i, zero_res);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
    r1 = s21_from_int_to_decimal(n1, &dec);
    r2 = s21_from_decimal_to_int(dec, &res1);
    ck_assert_int_eq(n1, res1);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
    r1 = s21_from_int_to_decimal(n2, &dec);
    r2 = s21_from_decimal_to_int(dec, &res2);
    ck_assert_int_eq(n2, res2);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
    r1 = s21_from_int_to_decimal(n2, &dec);
    r2 = s21_from_decimal_to_int(dec, &res2);
    ck_assert_int_eq(n2, res2);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
//    r1 = s21_from_decimal_to_int(big_dec, &res1);
//    ck_assert_int_eq(r1, 1);
    s21_set_scale(&big_dec, 15);
    s21_set_bit(&big_dec.bits[3], 31, 1);
    r2 = s21_from_decimal_to_int(big_dec, &res2);
    ck_assert_int_eq(r2, 0);
}
END_TEST

START_TEST(float_converters_test) {
    s21_decimal dec;
    float zero_f = 0.0, nmb1 = -654321.123456, nmb2 = 1.678e-10, zero_fl, res1, res2;
    int r1 = s21_from_float_to_decimal(nmb1, &dec);
    int r2 = s21_from_decimal_to_float(dec, &res1);
    ck_assert_float_eq_tol(nmb1, res1, 1e-6);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
    r1 = s21_from_float_to_decimal(nmb2, &dec);
    r2 = s21_from_decimal_to_float(dec, &res2);
    ck_assert_float_eq_tol(nmb2, res2, 1e-6);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
//    r1 = s21_from_float_to_decimal(zero_f, &dec);
//    r2 = s21_from_decimal_to_float(dec, &zero_fl);
//    ck_assert_float_eq_tol(zero_f, zero_fl, 1e-6);
//    ck_assert_int_eq(r1, 0);
//    ck_assert_int_eq(r2, 0);
    r1 = s21_from_float_to_decimal(1.36e-20, &dec);
    r2 = s21_from_decimal_to_float(dec, &res1);
    ck_assert_float_eq_tol(1.36e-20, res1, 1e-6);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
    r1 = s21_from_float_to_decimal(1.36e+20, &dec);
    r2 = s21_from_decimal_to_float(dec, &res1);
    ck_assert_int_eq(r1, 0);
    ck_assert_int_eq(r2, 0);
    r1 = s21_from_float_to_decimal(1e+29, &dec);
    ck_assert_int_eq(r1, 1);
    r1 = s21_from_float_to_decimal(1e-29, &dec);
    r2 = s21_from_decimal_to_float(dec, &zero_f);
    ck_assert_int_eq(r1, 1);
    ck_assert_int_eq(r2, 0);
    ck_assert_float_eq(zero_f, 0.0);
    r1 = s21_from_float_to_decimal(NaN, &dec);
    ck_assert_int_eq(r1, 1);
    r1 = s21_from_float_to_decimal(inf, &dec);
    ck_assert_int_eq(r1, 1);
    r1 = s21_from_float_to_decimal(neg_inf, &dec);
    ck_assert_int_eq(r1, 1);
    r1 = s21_from_float_to_decimal(neg_inf, &dec);
    ck_assert_int_eq(r1, 1);
}

START_TEST(test_s21_add) {
    int result_full1 = 0;
    result_full1 = s21_add(case_full1, case_full2, &result1);
    ck_assert_int_eq(result_full1, 1);
    s21_set_bit(&case_full1.bits[3], 31, 1);
    s21_decimal case_full = {{123, 321, 0xFFFFFFFF, 0}};
    result_full1 = s21_add(case_full, case_full, &result2);
    ck_assert_int_eq(result_full1, 1);
    s21_add(case_max, case_min, &result1);
    int int_res = -1, from_decimal;
    s21_decimal test;
    s21_from_int_to_decimal(int_res, &test);
    s21_from_decimal_to_int(test, &from_decimal);
    ck_assert_int_eq(from_decimal, -1);
    s21_add(case1, case2, &result2);
    s21_from_decimal_to_int(result2, &int_res);
    ck_assert_int_eq(int_res, 1);
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal result;
    result_full1 = s21_add(one, case_max, &result);
    ck_assert_int_eq(result_full1, 1);
}
END_TEST
// 2
START_TEST(test_s21_sub) {
    int int_res = -1;
    int res_full1 = 0;
    int res_full2 = 0;
    s21_decimal case_full = {{123, 321, 0xFFFFFFFF, 0}};
    res_full1 = s21_sub(case_full, case_full, &result1);
    ck_assert_int_eq(res_full1, 0);
    s21_set_bit(&case_full1.bits[3], 31, 1);
    res_full2 = s21_sub(case_full1, case_full2, &result2);
    ck_assert_int_eq(res_full2, 2);
    int_res = s21_sub(case1, case_max, &result1);
    ck_assert_int_eq(int_res, 0);
    s21_sub(case1, case2, &result2);
    s21_from_decimal_to_int(result2, &int_res);
    ck_assert_int_eq(int_res, -1);
}
END_TEST
// 3
START_TEST(test_s21_mul) {
    int res_mul = -1;
    s21_decimal case_d = {{0, 0, 0, 0}};
    res_mul = s21_mul(case_d, case_max, &result1);
    ck_assert_int_eq(res_mul, 0);
    s21_decimal max_neg = {{467756, 63754, 0xFFFFFFFF, 0}};
    s21_set_bit(&max_neg.bits[3], 31, 1);
    s21_decimal max_pos = {{467756, 63754, 0xFFFFFFFF, 0}};
    res_mul = s21_mul(max_neg, max_pos, &result1);
    ck_assert_int_eq(res_mul, 2);
    int res_ful1 = 0;
    int res_ful2 = 0;
    res_ful1 = s21_mul(case_full1, case_full2, &result1);
    ck_assert_int_eq(res_ful1, 1);
    res_ful2 = s21_mul(case_full1, case_full2, &result2);
    s21_set_bit(&result2.bits[3], 31, 1);
    ck_assert_int_eq(res_ful2, 1);
    s21_decimal val1, val2, res;
    float n1 = -5.6e-10, n2 = 1.5e-7;
    s21_from_float_to_decimal(n1, &val1);
    s21_from_float_to_decimal(n2, &val2);
    int r = s21_mul(val1, val2, &res);
    ck_assert_int_eq(r, 0);
    n1 = -5.6e-15;
    n2 = 1.5e-15;
    s21_from_float_to_decimal(n1, &val1);
    s21_from_float_to_decimal(n2, &val2);
    r = s21_mul(val1, val2, &res);
    ck_assert_int_eq(r, 0);
    ck_assert_int_eq(res.bits[0], 0);
    ck_assert_int_eq(res.bits[1], 0);
    ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

// 4
START_TEST(test_s21_div) {
    int int_res = -1;
    int res_div = -1;
    int_res = s21_div(case1, case_max, &result1);
    ck_assert_int_eq(int_res, 0);
    res_div = s21_div(case_max, case1, &result2);
    ck_assert_int_eq(res_div, 3);
    s21_div(case12, case13, &result2);
    int res_ful1 = 0;
    int res_ful2 = 0;
    res_ful1 = s21_div(case_full1, case_full2, &result1);
    ck_assert_int_eq(res_ful1, 0);
    s21_decimal one = {{1, 0, 0, 0}};
    res_ful2 = s21_div(one, case_max, &result2);
    ck_assert_int_eq(res_ful2, 2);
    res_ful1 = s21_mod(one, one, &result1);
    ck_assert_int_eq(res_ful1, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_1) {
    s21_decimal val;

    s21_from_float_to_decimal(0.03, &val);
    ck_assert_int_eq(val.bits[0], 3);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 131072);

    s21_from_float_to_decimal(127.1234, &val);
    ck_assert_int_eq(val.bits[0], 1271234);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 262144);
}
END_TEST
START_TEST(s21_from_float_to_decimal_2) {
    s21_decimal val;
    s21_from_float_to_decimal(-128.023, &val);
    ck_assert_int_eq(val.bits[0], 12802299);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(s21_get_bit(val.bits[3], 31), 1);
}
END_TEST
START_TEST(s21_from_float_to_decimal_3) {
    s21_decimal val;
    s21_from_float_to_decimal(-2.1474836E+09, &val);
    ck_assert_int_eq(val.bits[0], 2147483648);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(s21_get_bit(val.bits[3], 31), 1);
}
END_TEST
START_TEST(s21_from_float_to_decimal_4) {
    s21_decimal val;
    s21_from_float_to_decimal(22.14836E+03, &val);
    ck_assert_int_eq(val.bits[0], 2214836);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 131072);
}
END_TEST
START_TEST(s21_from_float_to_decimal_5) {
    s21_decimal val;
    s21_from_float_to_decimal(1.02E+09, &val);
    ck_assert_int_eq(val.bits[0], 1020000000);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(s21_from_float_to_decimal_6) {
    s21_decimal val;
    s21_from_float_to_decimal(-333.2222, &val);
    ck_assert_int_eq(val.bits[0], 3332222);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(s21_get_bit(val.bits[3], 31), 1);
}
END_TEST
START_TEST(s21_from_float_to_decimal_7) {
    s21_decimal val;
    float a = 1.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(s21_from_float_to_decimal_8) {
    s21_decimal val;
    float a = -1.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(s21_from_float_to_decimal_9) {
    s21_decimal val;
    float a = 0.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_1) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 18122;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147680256;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -18.122);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_2) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 1812;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -1812);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_3) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 0XFFFFFF;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 16777215);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_4) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 23450987;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147745792;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -2345.0987);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_5) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 4294967295;
    src.bits[1] = 4294967295;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
    ck_assert_int_eq(result, 0);
}
END_TEST

//START_TEST(s21_div_1) {
//    s21_decimal src1, src2, res_od;
//    float a = 9403.0e2;
//    int b = 202;
//    float res_our_dec = 0.0;
//    s21_from_float_to_decimal(a, &src1);
//    s21_from_int_to_decimal(b, &src2);
//    float res_origin = 4654.950495049504950495049505;
//    s21_div(src1, src2, &res_od);
//    s21_from_decimal_to_float(res_od, &res_our_dec);
//    ck_assert_float_eq(res_our_dec, res_origin);
//}
//END_TEST
START_TEST(s21_div_2) {
    s21_decimal src1, src2, res_od;
    int a = -32768;
    int b = 2;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -16384;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_3) {
    s21_decimal src1, src2, res_od;
    float a = -9403.0e2;
    float b = -2020.29;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = 465.4282306005573457275935633;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_4) {
    s21_decimal src1, src2, res_od;
    float a = -9403.0e2;
    float b = 2020.29;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = -465.4282306005573457275935633;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_5) {
    s21_decimal src1, src2, res_od;
    float a = -9403.0e20;
    float b = 2.28e17;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_6) {
    s21_decimal src1, src2, res_od;
    float a = -0.9;
    float b = 30.323;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_7) {
    s21_decimal src1, src2, res_od;
    float a = -0.9e3;
    float b = 30.32;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_8) {
    s21_decimal src1, src2, res_od;
    float a = -0.9e3;
    int b = 30;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = a / b;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_div_9) {
    s21_decimal src1, src2, res_od;
    float a = -0.9;
    float b = 0.000076;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = a / b;
    s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(s21_div_10) {
    s21_decimal src1, src2, res_od;
    float a = 1.0 / 0.0;
    float b = 1.0 / 0.0;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    int r = s21_div(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_int_eq(r, 3);
    ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
}
END_TEST

START_TEST(s21_div_11) {
    s21_decimal src1, src2, res_od;
    int a = -17272;
    float b = -1.0 / 0.0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    s21_div(src1, src2, &res_od);
    s21_decimal zero = {{0, 0, 0, 0}};
    ck_assert_int_eq(!!(s21_is_equal(res_od, zero) == 1), 1);
    ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
}
END_TEST

START_TEST(s21_div_12) {
    s21_decimal src1, src2, res_od;
    float a = -115.2;
    float b = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    int r = s21_div(src1, src2, &res_od);

    ck_assert_int_eq(r, 3);
    ck_assert_int_eq(res_od.bits[3], 0);
    ck_assert_int_eq(res_od.bits[2], 0);
    ck_assert_int_eq(res_od.bits[1], 0);
    ck_assert_int_eq(res_od.bits[0], 0);
}
END_TEST

START_TEST(s21_mod_1) {
    s21_decimal src1, src2, res_mod;
    int a = 3;
    int b = 2;
    int res_origin = a % b;
    int res = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_2) {
    s21_decimal src1, src2, res_mod;
    int a = 98765;
    int b = -1234;
    int res_origin = a % b;
    int res = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_3) {
    s21_decimal src1, src2, res_mod;
    int a = 30198;
    int b = 20210;
    int res_origin = a % b;
    int res = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_4) {
    s21_decimal src1, src2, res_mod;
    int a = -98765;
    int b = -1234;
    int res_origin = a % b;
    int res = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_5) {
    s21_decimal src1, src2, res_mod;
    int a = 98765;
    int b = 127234;
    int res_origin = a % b;
    int res = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_6) {
    s21_decimal src1, src2, res_mod;
    int a = 342576;
    int b = 1542134;
    int res_origin = a % b;
    int res = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_7) {
    s21_decimal src1, src2, res_mod;
    float a = 1.2;
    float b = 0.3;
    float res_origin = fmod(a, b);
    float res = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_8) {
    s21_decimal src1, src2, res_mod;
    float a = 1.2;
    int b = 3;
    float res_origin = fmod(a, b);
    float res = 0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    s21_mod(src1, src2, &res_mod);
    s21_from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mul_1) {
    s21_decimal src1, src2;
    float a = 9403.0e2;
    int b = 202;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    float res_origin = 189940600;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_mul_2) {
    s21_decimal src1, src2;
    float a = 9403.0e2;
    float b = 9403.0e2;
    float res_our_dec = 0.0;
    s21_from_float_to_decimal(a, &src1);
    s21_from_float_to_decimal(b, &src2);
    float res_origin = 884164090000;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_mul_3) {
    s21_decimal src1, src2;
    int a = 9403;
    int b = 202;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = 1899406;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_mul_4) {
    s21_decimal src1, src2;
    int a = -32768;
    int b = 2;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -65536;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST
START_TEST(s21_mul_5) {
    s21_decimal src1, src2;
    int a = -32768;
    int b = 32768;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -1073741824;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST

int main() {
    Suite *s1 = suite_create("Core");
    TCase *s21_test = tcase_create("Test");
    SRunner *sr = srunner_create(s1);
    suite_add_tcase(s1, s21_test);

    tcase_add_test(s21_test, is_less_test);
    tcase_add_test(s21_test, is_less_or_equal_test);
    tcase_add_test(s21_test, is_greater_test);
    tcase_add_test(s21_test, is_greater_or_equal_test);
    tcase_add_test(s21_test, is_equal_test);
    tcase_add_test(s21_test, is_not_equal_test);

    tcase_add_test(s21_test, s21_round_test);
    tcase_add_test(s21_test, s21_floor_test);

    tcase_add_test(s21_test, int_converters_test);
    tcase_add_test(s21_test, float_converters_test);

    tcase_add_test(s21_test, test_s21_add);
    tcase_add_test(s21_test, test_s21_sub);
    tcase_add_test(s21_test, test_s21_mul);
    tcase_add_test(s21_test, test_s21_div);

    tcase_add_test(s21_test, s21_from_float_to_decimal_1);
    tcase_add_test(s21_test, s21_from_float_to_decimal_2);
    tcase_add_test(s21_test, s21_from_float_to_decimal_3);
    tcase_add_test(s21_test, s21_from_float_to_decimal_4);
    tcase_add_test(s21_test, s21_from_float_to_decimal_5);
    tcase_add_test(s21_test, s21_from_float_to_decimal_6);
    tcase_add_test(s21_test, s21_from_float_to_decimal_7);
    tcase_add_test(s21_test, s21_from_float_to_decimal_8);
    tcase_add_test(s21_test, s21_from_float_to_decimal_9);

    tcase_add_test(s21_test, s21_from_decimal_to_float_1);
    tcase_add_test(s21_test, s21_from_decimal_to_float_2);
    tcase_add_test(s21_test, s21_from_decimal_to_float_3);
    tcase_add_test(s21_test, s21_from_decimal_to_float_4);
    tcase_add_test(s21_test, s21_from_decimal_to_float_5);

//    tcase_add_test(s21_test, s21_div_1);
    tcase_add_test(s21_test, s21_div_2);
    tcase_add_test(s21_test, s21_div_3);
    tcase_add_test(s21_test, s21_div_4);
    tcase_add_test(s21_test, s21_div_5);
    tcase_add_test(s21_test, s21_div_6);
    tcase_add_test(s21_test, s21_div_7);
    tcase_add_test(s21_test, s21_div_8);
    tcase_add_test(s21_test, s21_div_9);
    tcase_add_test(s21_test, s21_div_10);
    tcase_add_test(s21_test, s21_div_11);
    tcase_add_test(s21_test, s21_div_12);

    tcase_add_test(s21_test, s21_mod_1);
    tcase_add_test(s21_test, s21_mod_2);
    tcase_add_test(s21_test, s21_mod_3);
    tcase_add_test(s21_test, s21_mod_4);
    tcase_add_test(s21_test, s21_mod_5);
    tcase_add_test(s21_test, s21_mod_6);
    tcase_add_test(s21_test, s21_mod_7);
    tcase_add_test(s21_test, s21_mod_8);

    tcase_add_test(s21_test, s21_mul_1);
    tcase_add_test(s21_test, s21_mul_2);
    tcase_add_test(s21_test, s21_mul_3);
    tcase_add_test(s21_test, s21_mul_4);
    tcase_add_test(s21_test, s21_mul_5);

    srunner_run_all(sr, CK_VERBOSE);
    int errors = srunner_ntests_failed(sr);
    srunner_free(sr);

    return errors == 0 ? 0 : 1;
}
