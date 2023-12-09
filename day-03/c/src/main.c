#include <stdarg.h>

#define FC_IMPLEMENTATION
#include "../../../lib/c/fc.h"

#define GET_INPUT_IMPLEMENTATION
#include "../../../lib/c/get_input.h"

int part1(SV input) {
    int total = 0;

    SV clone = sv_clone(input);
    usize width = sv_split(&clone, '\n').count;
    usize height = (input.count + 1) / (width + 1); // +1 to account for newlines

    SV_FOR_EACH_SPLIT(y, line, NL_SV, &input) {

        usize start = -1;
        usize end = -1;
        usize count = 0;

        SV_FOR_EACH_SPLIT(x, data, EMPTY_SV, &line) {
            char c = data.data[0];

            if (isdigit(c)) {
                if (start == (usize)-1) {
                    start = x;
                }

                end = x;
                count = count * 10 + (c - '0');
            }

            if (!isdigit(c) || x == width - 1) {
                if (start != (usize)-1) {
                    usize x1 =  start > 0 ? start - 1 : start;
                    usize x2 = end < width - 1 ? end + 1 : end;

                    usize y1 = y > 0 ? y - 1 : y;
                    usize y2 = y < height - 1 ? y + 1 : y;

                    bool valid = false;

                    for (usize y = y1; y <= y2; y++) {
                        for (usize x = x1; x <= x2; x++) {
                            char test = input.data[y * (width + 1) + x];

                            if (test != '.' && !isdigit(test)) {
                                valid = true;
                                goto out;
                            }
                        }
                    }

                    out:

                    if (valid) {
                        // printf("%d", count);
                        total += count;
                    }
                }

                start = -1;
                end = -1;
                count = 0;
            }
        }
    }

    return total;
}

typedef struct {
    usize x;
    usize y;
    usize value;
} Element;

da_typedef(Element, Elements);

int part2(SV input) {
    int total = 0;

    SV clone = sv_clone(input);
    usize width = sv_split(&clone, '\n').count;
    usize height = (input.count + 1) / (width + 1); // +1 to account for newlines

    Elements elements = {0};

    SV_FOR_EACH_SPLIT(y, line, NL_SV, &input) {

        usize start = -1;
        usize end = -1;
        usize count = 0;

        SV_FOR_EACH_SPLIT(x, data, EMPTY_SV, &line) {
            char c = data.data[0];

            if (isdigit(c)) {
                if (start == (usize)-1) {
                    start = x;
                }

                end = x;
                count = count * 10 + (c - '0');
            }

            if (!isdigit(c) || x == width - 1) {
                if (start != (usize)-1) {
                    usize x1 =  start > 0 ? start - 1 : start;
                    usize x2 = end < width - 1 ? end + 1 : end;

                    usize y1 = y > 0 ? y - 1 : y;
                    usize y2 = y < height - 1 ? y + 1 : y;

                    for (usize y = y1; y <= y2; y++) {
                        for (usize x = x1; x <= x2; x++) {
                            char test = input.data[y * (width + 1) + x];

                            if (test == '*') {
                                for (usize i = 0; i < elements.count; i++) {
                                    Element* element = &elements.items[i];

                                    if (element->x == x && element->y == y) {
                                        total += element->value * count;
                                        goto out;
                                    }
                                }

                                Element element = {x, y, count};
                                da_push(&elements, element);
                            }
                        }
                    }
                }

                out:

                start = -1;
                end = -1;
                count = 0;
            }
        }
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

    printf("Part 1: %d\n", part1(sv_from_cstr(input)));
    printf("Part 2: %d\n", part2(sv_from_cstr(input)));
}

void test() {
    printf("Testing part 1: ");
    fflush(stdout);

    ASSERT_EQ(int, part1(sv_from_cstr("467..114..\n\
...*......\n\
..35..633.\n\
......#...\n\
617*......\n\
.....+.58.\n\
..592.....\n\
......755.\n\
...$.*....\n\
.664.598..")), 4361, %d, %d);

    printf("PASSED\nTesting part 2: ");
    fflush(stdout);

    ASSERT_EQ(int, part2(sv_from_cstr("467..114..\n\
...*......\n\
..35..633.\n\
......#...\n\
617*......\n\
.....+.58.\n\
..592.....\n\
......755.\n\
...$.*....\n\
.664.598..")), 467835, %d, %d);


    printf("PASSED\n\nBoth tests passed!\n");
}