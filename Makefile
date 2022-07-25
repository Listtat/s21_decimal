OS := $(shell uname -s)
CC = gcc
FLAG = -Wall -Werror -Wextra
FLAG_C = -c -Wall -Werror -Wextra
CL_11 = -std=c11
ALL_FILE = s21_helpers.c s21_converters.c s21_arithmetic.c s21_another.c s21_comparison.c
ALL_FILE_O = s21_helpers.o s21_converters.o s21_arithmetic.o s21_another.o s21_comparison.o

ifeq ($(OS), Darwin)
	FLAGS_PLATFORM = -lcheck
else
	FLAGS_PLATFORM = -lcheck -lsubunit -lrt -lm -lpthread
endif

all: clean s21_decimal.a

s21_decimal.a:
	@$(CC) $(FLAG_C) $(ALL_FILE)
	@ar rcs s21_decimal.a $(ALL_FILE_O)

test: s21_decimal.a
	@gcc unit_test.c s21_decimal.a $(FLAGS_PLATFORM) -g -o tests.o
	@./tests.o

gcov_report:
	@$(CC) --coverage $(ALL_FILE) unit_test.c $(FLAGS_PLATFORM) -o gcov_report.o
	@./gcov_report.o
	@lcov -t s21_decimal_tests -o s21_decimal_tests.info -c -d .
	@genhtml -o report s21_decimal_tests.info
	@open report/index.html

clean:
	@rm -f *.a
	@rm -f *.o
	@rm -f *.gcda
	@rm -f *.gcdo
	@rm -f *.gcno
	@rm -f *.info

leaks: $(ALL_FILE_O)
	@valgrind --trace-children=yes --track-fds=yes --leak-check=full --show-leak-kinds=all --show-reachable=no --log-file=RESULT_VALGRIND.txt ./tests.o
#	@CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./tests.o

style: $(ALL_FILE)
	@cp ../materials/linters/CPPLINT.cfg ./
	@python3 ./../materials/linters/cpplint.py --extensions=c $(ALL_FILE) unit_test.c
	@rm -f CPPLINT.cfg

cppcheck: $(ALL_FILE_O)
	@cppcheck --enable=all --suppress=missingIncludeSystem . $(ALL_FILE)

check: style cppcheck
