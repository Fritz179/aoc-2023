#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <alloca.h>

#define GET_INPUT_IMPLEMENTATION
#include "../../../lib/c/get_input.h"

#define FC_IMPLEMENTATION
#include "../../../lib/c/fc.h"

u64 MAX_RED = 12;
u64 MAX_GREEN = 13;
u64 MAX_BLUE = 14;

int part1(SV input) {
    int total = 0;

    while (!sv_is_empty(input)) {
        sv_take_sv(&input, sv_from_cstr("Game "));
        u64 id = sv_parse_u64(&input);
        sv_take_sv(&input, sv_from_cstr(": "));

        bool possible = true;

        while (!sv_is_empty(input) && !sv_starts_with(input, '\n')) {
            u64 red = 0;
            u64 green = 0;
            u64 blue = 0;

            while (!sv_is_empty(input) && !sv_starts_with(input, '\n') && !sv_starts_with(input, ';')) {
                u64 count = sv_parse_u64(&input);

                if (sv_take_sv(&input, sv_from_cstr(" red"))) {
                    if (count > red) {
                        red = count;
                    }
                } else if (sv_take_sv(&input, sv_from_cstr(" green"))) {
                    if (count > green) {
                        green = count;
                    }
                } else if (sv_take_sv(&input, sv_from_cstr(" blue"))) {
                    if (count > blue) {
                        blue = count;
                    }
                }

                sv_take(&input, ',');
                sv_take(&input, ' ');
            }
            
            if (red > MAX_RED || green > MAX_GREEN || blue > MAX_BLUE) {
                possible = false;
            }

            sv_take(&input, ';');
            sv_take(&input, ' ');
        }

        if (possible) {
            total += id;
        }

        sv_take(&input, '\n');
    }

    return total;
}

int part2(SV input) {
    u64 total = 0;

    SV nl = sv_from_cstr("\n");
    SV_FOR_EACH_SPLIT(_, line, nl, &input) {
        sv_take_sv(&line, sv_from_cstr("Game "));
        sv_parse_u64(&line);
        sv_take(&line, ':');

        u64 red = 0;
        u64 green = 0;
        u64 blue = 0;

        SV col = sv_from_cstr(";");
        SV_FOR_EACH_SPLIT(_, game, col, &line) {

            SV com = sv_from_cstr(",");
            SV_FOR_EACH_SPLIT(_, card, com, &game) {
                u64 count = sv_parse_u64(&card);

                if (sv_eq(card, sv_from_cstr(" red"))) {
                    if (count > red) {
                        red = count;
                    }
                } else if (sv_eq(card, sv_from_cstr(" green"))) {
                    if (count > green) {
                        green = count;
                    }
                } else if (sv_eq(card, sv_from_cstr(" blue"))) {
                    if (count > blue) {
                        blue = count;
                    }
                } else {
                    UNREACHABLE("not possible");
                }
            }
        }

        total += red * blue * green;
    }

    return (u32)total;
}

void test();
int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        test();
        return 0;
    }

    char* input = get_input(argc, argv);

    printf("Part 1: %d\n", part1(sv_from_cstr(input)));
    printf("Part 2: %d\n", part2(sv_from_cstr(input)));
}

void test() {
    printf("Testing part 1: ");
    fflush(stdout);

    ASSERT_EQ(int, part1(sv_from_cstr("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n\
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n\
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n\
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n\
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green")), 8, %d, %d);

    printf("PASSED\nTesting part 2: ");
    fflush(stdout);

    ASSERT_EQ(int, part2(sv_from_cstr("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n\
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n\
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n\
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n\
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green")), 2286, %d, %d);


    printf("PASSED\n\nBoth tests passed!\n");
}