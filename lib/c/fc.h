// Copyright 2023 Fabrizio Cortesi <fabrizio.cortesi179@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef FC_H
#define FC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

// Types
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define i8 signed char
#define i16 signed short
#define i32 signed int
#define i64 signed long long

#define f32 float
#define f64 double

#define usize size_t
#define isize ptrdiff_t

// Common stuff
#define SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)

#define TODO(...)                                                      \
    do {                                                               \
        printf("%s:%d: TODO: %s \n", __FILE__, __LINE__, __VA_ARGS__); \
        exit(1);                                                       \
    } while(0)

#define UNREACHABLE(...)                                                      \
    do {                                                                      \
        printf("%s:%d: UNREACHABLE: %s \n", __FILE__, __LINE__, __VA_ARGS__); \
        exit(1);                                                              \
    } while(0)

#define ASSERT(a)                                                      \
    do {                                                                      \
        if (!a) { \
            printf("%s:%d: Assertion failed!", __FILE__, __LINE__); \
            printf(" `assert(%s)`\n", #a); \
            printf("      value: '%d'\n", a); \
            exit(1);                                                              \
        } \
    } while(0)

#define ASSERT_EQ(type, a, b, fmt_a, fmt_b)                                                      \
    do {                         \
        type value_a = a; \
        type value_b = b; \
        if (value_a != value_b) { \
            printf("%s:%d: Assertion failed!", __FILE__, __LINE__); \
            printf(" `(%s == %s)`\n", #a, #b); \
            printf("       left: '" #fmt_a "'\n", value_a); \
            printf("      right: '" #fmt_b "'\n", value_b); \
            exit(1);                                                              \
        } \
    } while(0)

#define ASSERT_SV_EQ(sv_a, sv_b) \
    do { \
        SV a = (sv_a); \
        SV b = (sv_b); \
        if (sv_eq(a, b)) { \
            break; \
        } \
 \
        printf("%s:%d: Assertion failed!", __FILE__, __LINE__); \
        if (a.count != b.count) { \
            printf(" `(%s == %s)`\n", #sv_a, #sv_b); \
            printf("       left has a length of: '%zu'\n", a.count); \
            printf("      right has a length of: '%zu'\n\n", b.count); \
        } \
 \
        usize i = 0; \
        for (; i < a.count; i++) { \
            if (a.data[i] != b.data[i]) { \
                break; \
            } \
        } \
 \
        int next_a = i + 10 > a.count ? a.count - i : 10; \
        int next_b = i + 10 > b.count ? b.count - i : 10; \
        printf("They both match until index: '%zu'\n", i); \
        printf("        preciding: '" SV_FMT"'\n", (int)i, a.data); \
        printf("         lhs (%d): '" SV_FMT"'\n", next_a, next_a, a.data + i); \
        printf("         rhs (%d): '" SV_FMT"'\n", next_b, next_b, b.data + i); \
        printf("    lhs (%d)(escaped): '", next_a); \
        for (int ai = 0; ai < next_a; ai++) { \
            print_escaped_char(a.data[ai + i]); \
        } \
        printf("'\n    rhs (%d)(escaped): '", next_b); \
        for (int bi = 0; bi < next_b; bi++) { \
            print_escaped_char(b.data[bi + i]); \
        } \
        printf("'\n"); \
            exit(1); \
    } while(0)


// Dynamic array
#ifndef DA_INIT_CAP
#define DA_INIT_CAP 4
#endif

#define da_typedef(item, name)  \
    typedef struct {            \
        item* items;            \
        usize count;           \
        usize capacity;        \
    } name

#define da_init(da, name) \
    da name = {0};          

#define da_last(da) (assert((da)->count > 0), (da)->items[(da)->count - 1])

#define da_push(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Malloc failed :-(");                      \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

#define da_push_sa(da, new_items)                                      \
    do {                                 \
        usize new_items_count = sizeof(new_items) / sizeof(*(da)->items);                  \
        da_push_count(da, new_items, new_items_count);                                    \
    } while (0)

#define da_push_count(da, new_items, new_items_count)                                     \
    do {                                                                                    \
        if ((da)->count + new_items_count > (da)->capacity) {                               \
            if ((da)->capacity == 0) {                                                      \
                (da)->capacity = DA_INIT_CAP;                                               \
            }                                                                               \
            while ((da)->count + new_items_count > (da)->capacity) {                        \
                (da)->capacity *= 2;                                                        \
            }                                                                               \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));        \
            assert((da)->items != NULL && "Malloc failed :-(");                             \
        }                                                                                   \
        memcpy((da)->items + (da)->count, new_items, new_items_count*sizeof(*(da)->items)); \
        (da)->count += new_items_count;                                                     \
    } while (0)

#define da_print(da, item, print) \
    do { \
        printf("da(occupied: %zu/%zu) [", (da)->count, (da)->capacity); \
        for (usize i = 0; i < (da)->count; i++) { \
            if (i != 0) { \
                printf(", "); \
            } \
            item = (da)->items[i]; \
            print; \
        } \
        printf("]\n"); \
    } while (0)

#define da_pretty_print(da, item, print) \
    do { \
        printf("da(occupied: %zu/%zu) [\n    ", (da)->count, (da)->capacity); \
        for (usize i = 0; i < (da)->count; i++) { \
            if (i != 0) { \
                printf(",\n    "); \
            } \
            item = (da)->items[i]; \
            print; \
        } \
        printf("\n]\n"); \
    } while (0)


// String builder
da_typedef(char, StringBuilder);

// String view
typedef struct {
    const char *data;
    usize count;
} SV;

void sb_append_cstr(StringBuilder* sb, const char* cstr);
void sb_append_null(StringBuilder* sb);
void sb_append(StringBuilder* sb, char c);
void sb_append_sv(StringBuilder* sb, SV sv);

void sb_print(StringBuilder sb);

SV sb_to_sv(StringBuilder sb);


#define SV_FMT "%.*s"
#define SV_ARG(sv) (int) (sv).count, (sv).data
// USAGE:
//   String_View name = ...;
//   printf("Name: "SV_FMT"\n", SV_ARG(name));

SV EMPTY_SV;
SV NL_SV;

// Creating new SVs
SV sv_from_parts(const char *data, usize count);
SV sv_from_cstr(const char *cstr);
SV sv_clone(SV sv);

// Trimming => remove matching
usize sv_trim_left(SV* sv);
usize sv_trim_right(SV* sv);
usize sv_trim(SV* sv);

// Splitting => remove until matching, return removed
SV sv_split(SV* sv, char c);
SV sv_split_sv(SV* sv, SV delim);

// Take => remove once, return if removed
bool sv_take(SV* sv, char c);
bool sv_take_sv(SV* sv, SV delim);

// Tests
bool sv_eq(SV a, SV b);
bool sv_is_empty(SV sv);
bool sv_starts_with(SV sv, char prefix);
bool sv_starts_with_sv(SV sv, SV prefix);

// Parsing
u64 sv_parse_u64(SV* sv);

// Debugging
void sv_print(SV sv);
void sv_print_escaped(SV sv);

// TODO: smarter looping
// index: enumerated i
// window: current SV
// delimeter: splitting SV
#define SV_FOR_EACH_SPLIT(index, window, delimeter, sv) \
SV input_clone = sv_clone(*sv); \
usize index = 0;\
usize left = input_clone.count + delimeter.count;\
SV window = sv_split_sv(&input_clone, delimeter); \
usize window_len = window.count; \
for (; left > 0; left = left - window_len - delimeter.count, window = sv_split_sv(&input_clone, delimeter), window_len = window.count, index++)

 // #define SV_FOR_EACH_LINE(index, line, sv) SV_FOR_EACH_SPLIT(index, line, NL_SV, sv)

#endif // FC_H

#ifdef FC_IMPLEMENTATION

SV EMPTY_SV = {0};
SV NL_SV = { "\n", 1 };

void sb_append_cstr(StringBuilder* sb, const char* cstr) {
    usize n = strlen(cstr);     
    da_push_count(sb, cstr, n); 
}

void sb_append_null(StringBuilder* sb) {
    da_push_count(sb, "", 1);
}

void sb_append(StringBuilder* sb, char c) {
    da_push(sb, c);
}

void sb_append_sv(StringBuilder* sb, SV sv) {
    da_push_count(sb, sv.data, sv.count);
}

void sb_print(StringBuilder sb) {
    printf("StringBuilder(occupied: %zu/%zu)["SV_FMT"]\n", sb.count, sb.capacity, (int)sb.count, sb.items);
}

SV sb_to_sv(StringBuilder sb) {
    return sv_from_parts(sb.items, sb.count);
}

// Create a String View from a pointer and a count
SV sv_from_parts(const char *data, usize count) {
    return (SV){data, count};
}

// Create a String View from a null-terminated string
SV sv_from_cstr(const char *cstr) {
    return sv_from_parts(cstr, strlen(cstr));
}

// Clone a String View
SV sv_clone(SV sv) {
    return sv_from_parts(sv.data, sv.count);
}

// Remove whitespace from the left of sv
// Returns the number of characters removed
usize sv_trim_left(SV* sv) {
    usize total = sv->count;

    while (sv->count && isspace(sv->data[0])) {
        sv->data++;
        sv->count--;
    }

    return total - sv->count;
}

// Remove whitespace from the right of sv
// Returns the number of characters removed
usize sv_trim_right(SV* sv) {
    usize total = sv->count;

    while (sv->count && isspace(sv->data[sv->count - 1])) {
        sv->data++;
        sv->count--;
    }

    return total - sv->count;
}

// Remove whitespace from the left and right of sv
// Returns the number of characters removed
usize sv_trim(SV* sv) {
    return sv_trim_left(sv) + sv_trim_right(sv);
}

// Remove characters from the left of sv until c is found
// Returns the removed characters
SV sv_split(SV* sv, char c) {
    const char* start = sv->data;

    while (sv->count && sv->data[0] != c) {
        sv->data++;
        sv->count--;
    }

    // don't include the delimiter
    SV chopped = sv_from_parts(start, sv->data - start);

    // Remove the delimiter 
    if (sv->count) {
        sv->data++;
        sv->count--;
    }

    return chopped;
}

// Remove characters from the left of sv until delimeter is found
// Returns the removed characters
SV sv_split_sv(SV* sv, SV delimeter) {
    const char* start = sv->data;

    if (delimeter.count) {

        // Non-empty delimeter, remove until it's found
        while (sv->count >= delimeter.count && !sv_eq(sv_from_parts(sv->data, delimeter.count), delimeter)) {
            sv->data++;
            sv->count--;
        }
    } else if (sv->count) {

        // Empty delimeter, remove only one character
        sv->data++;
        sv->count--;
    }

    if (sv->count < delimeter.count) {
        sv->data += sv->count;
        sv->count = 0;
    }

    // don't include the delimiter
    SV chopped = sv_from_parts(start, sv->data - start);

    // Remove the delimiter 
    if (sv->count) {
        sv->data += delimeter.count;
        sv->count -= delimeter.count;
    }

    return chopped;
}

// Remove the first character of sv if it matches c
// Returns true if the character was removed
bool sv_take(SV* sv, char c) {
    if (sv->count && sv->data[0] == c) {
        sv->data++;
        sv->count--;
        return true;
    }

    return false;
}

// Remove the first delimeter of sv if it matches delimeter
// Returns true if the delimeter was removed
bool sv_take_sv(SV* sv, SV delimeter) {
    if (sv->count >= delimeter.count && sv_eq(sv_from_parts(sv->data, delimeter.count), delimeter)) {
        sv->data += delimeter.count;
        sv->count -= delimeter.count;
        return true;
    }

    return false;
}

// Check if two SVs are equal
bool sv_eq(SV a, SV b) {
    if (a.count != b.count) {
        return false;
    }

    for (usize i = 0; i < a.count; i++) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }

    return true;
}

// Check if an SV is empty
bool sv_is_empty(SV sv) {
    return sv.count == 0;
}

// Check if an SV starts with a prefix
bool sv_starts_with(SV sv, char prefix) {
    return sv.count && sv.data[0] == prefix;
}


// Check if an SV starts with a sv prefix
bool sv_starts_with_sv(SV sv, SV prefix) {
    if (sv.count != prefix.count) {
        return false;
    }

    for (usize i = 0; i < prefix.count; i++) {
        if (sv.data[i] != prefix.data[i]) {
            return false;
        }
    }

    return true;
}

// TODO: handle overflow
// TODO: handle not a number
// Remove u64 from the left of sv
// Returns the u64
u64 sv_parse_u64(SV* sv) {
    u64 result = 0;

    sv_trim_left(sv);

    while (sv->count && isdigit(sv->data[0])) {
        result = result * 10 + sv->data[0] - '0';
        sv->data++;
        sv->count--;
    }

    return result;
}

void sv_print(SV sv) {
    printf("SV(size: %zu)["SV_FMT"]\n", (sv).count, SV_ARG((sv)));
}

void print_escaped_char(char c) {
    switch (c) {
        case '\n': printf("\\n"); break;
        case '\r': printf("\\r"); break;
        case '\t': printf("\\t"); break;
        case '\v': printf("\\v"); break;
        case '\b': printf("\\b"); break;
        case '\f': printf("\\f"); break;
        case '\a': printf("\\a"); break;
        case '\\': printf("\\\\"); break;
        default: 
            if (c >= 32 && c < 127) {
                printf("%c", c);
            } else {
                printf("\\x%02x", c);
            }
    }

}

void sv_print_escaped(SV sv) {
    printf("SV(size: %zu)(escaped)[", sv.count);

    for (usize i = 0; i < sv.count; i++) {
        print_escaped_char(sv.data[i]);
    }

    printf("]\n");
}

#endif // FC_IMPLEMENTATION

#ifdef FC_TESTS

void do_fc_tests() {
    printf("Running fc tests...\n");
    
    SV input = sv_from_cstr(":A:B::C:");
    SV delim = sv_from_cstr(":");

    StringBuilder out = {0};
    char* buffer = malloc(1024);
    SV_FOR_EACH_SPLIT(i, window, delim, &input) {
        sprintf(buffer, "i: %zu, window: "SV_FMT"\n", i, SV_ARG(window));
        sb_append_cstr(&out, buffer);
    }

    SV sv = sb_to_sv(out);

    ASSERT_SV_EQ(sv, sv_from_cstr("i: 0, window: \n\
i: 1, window: A\n\
i: 2, window: B\n\
i: 3, window: \n\
i: 4, window: C\n\
i: 5, window: \n"));

    printf("fc tests passed!\n");

}

#endif // FC_TESTS

// TODO: align all \ at end of line
// TODO: add parenthesis around proc macro arguments
// TODO: make UNREACHABLE accept fmt and args
// TODO: Add SV_FOR_EACH_LINE and SV_FOR_EACH_CHAR
// TODO: Add DA_FIND / DA_EVERY / DA_ANY...
// TODO: add tests lol