#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define GET_INPUT_IMPLEMENTATION
#include "../../../lib/c/get_input.h"

bool starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return false;
        }
    }

    return true;
}

int part1(char* input) {
    int MAX_RED = 12;
    int MAX_GREEN = 13;
    int MAX_BLUE = 14;

    int total = 0;

    while (*input) {

        input += strlen("Game ");
        int id;
        sscanf(input, "%d", &id);
        while (*input != ' ') {
            input++;
        }

        bool possible = true;

        while (*input != '\n' && *input) {
            printf("\n\n%s", input);
            int red = 0;
            int green = 0;
            int blue = 0;

            while (*input != ';' && *input == '\n' && *input) {
                input++;
                int count; 
                sscanf(input, "%d", &count);
                while (*input != ' ') {
                    input++;
                }
                

                if (starts_with(input, " red")) {
                    if (count > red) {
                        red = count;
                    }
                } else if (starts_with(input, " green")) {
                    if (count > green) {
                        green = count;
                    }
                } else if (starts_with(input, " blue")) {
                    if (count > blue) {
                        blue = count;
                    }
                }
            }
            
            if (red > MAX_RED || green > MAX_GREEN || blue > MAX_BLUE) {
                possible = false;
            }

            if (*input == '\n') {
                input++;
            }
        }
    }

    return total;
}

int part2(char* input) {
    return 1;
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

    assert(part1("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n\
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n\
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n\
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n\
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green") == 8);

    printf("PASSED\nTesting part 2: ");
    fflush(stdout);

    assert(part2("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n\
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n\
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n\
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n\
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green") == 2286);


    printf("PASSED\n\nBoth tests passed!\n");
}