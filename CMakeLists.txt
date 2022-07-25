cmake_minimum_required(VERSION 3.21)
project(C5 C)

set(CMAKE_C_STANDARD 11)

#set(SOURCES s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c unit_test.c s21_decimal.h)
#set(SOURCES Shahzod_convertors.c Shahzod_arithmetics.c Shahzod_comparisons.c Shahzod_decimal.h Shahzod_extra_func.c Shahzod_others.c)
#set(SOURCES for_debug.c s21_decimal.h)
set(SOURCES for_debug2.c s21_decimal.h)
#set(SOURCES Misha_debug.c Misha_decimal.h)
#set(SOURCES test.c)
include_directories(.)

add_executable(C5 ${SOURCES})

target_link_libraries(C5 check -g)