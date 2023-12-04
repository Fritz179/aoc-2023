#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../../lib/c/get_input.h"

char *NUMEBERS[9] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

int part1(const char *input) {
    int total = 0;

    while (*input) {
        int first = 0;
        int last = 0;
        char c;

        while ((c = *input++)) {
            c -= '0';

            if (c >= 1 && c <= 9) {
                if (!first) {
                    first = c;
                }

                last = c;
            }

            if (*input == '\n') {
                input++;
                break;
            }
        }

        total += first * 10 + last;
    }

    return total;
}

bool starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return false;
        }
    }

    return true;
}

int part2(const char *input) {
        int total = 0;

    while (*input) {
        int first = 0;
        int last = 0;
        char c;

        while ((c = *input)) {
            c -= '0';

            if (c >= 1 && c <= 9) {
                if (!first) {
                    first = c;
                }

                last = c;
            }

            for (int i = 0; i < 9; i++) {
                if (starts_with(input, NUMEBERS[i])) {
                    if (!first) {
                        first = i + 1;
                    }

                    last = i + 1;
                }
            }

            input++;
            if (*input == '\n') {
                input++;
                break;
            }
        }

        total += first * 10 + last;
    }

    return total;
}

void test();
int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        test();
        return 0;
    }

    char* input = get_input(argc, argv);

    printf("Part 1: %d\n", part1(input));
    printf("Part 2: %d\n", part2(input));

    return 0;
}

void test() {
    printf("Testing part 1: ");
    fflush(stdout);

    assert(part1("1abc2\n\
pqr3stu8vwx\n\
a1b2c3d4e5f\n\
treb7uchet") == 142);

    printf("PASSED\nTesting part 2: ");
    fflush(stdout);

    assert(part2("two1nine\n\
eightwothree\n\
abcone2threexyz\n\
xtwone3four\n\
4nineeightseven2\n\
zoneight234\n\
7pqrstsixteen") == 281);


    printf("PASSED\n\nBoth tests passed!\n");
}