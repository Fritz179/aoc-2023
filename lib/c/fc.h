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

#define ASSERT(a, fmt)                                                      \
    do {                                                                      \
        if (!a) { \
            printf("%s:%d: Assertion failed!", __FILE__, __LINE__); \
            printf(" `assert(%s)`\n", #a); \
            printf("      value: '" #fmt "'\n", a); \
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

#define da_append(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Malloc failed :-(");                      \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

#define da_append_sa(da, new_items)                                      \
    do {                                 \
        usize new_items_count = sizeof(new_items) / sizeof(*(da)->items);                  \
        da_append_count(da, new_items, new_items_count);                                    \
    } while (0)

#define da_append_count(da, new_items, new_items_count)                                     \
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

#define sb_append_cstr(sb, cstr)  \
    do {                          \
        const char *s = (cstr);   \
        usize n = strlen(s);     \
        da_append_many(sb, s, n); \
    } while (0)

#define sb_append_null(sb) da_append_many(sb, "", 1)

#define sb_to_sv(sb) sv_from_parts((sb).items, (sb).count)

// String view
typedef struct {
    const char *data;
    usize count;
} SV;

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int) (sv).count, (sv).data
// USAGE:
//   String_View name = ...;
//   printf("Name: "SV_FMT"\n", SV_ARG(name));


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

// TODO: smarter looping
// index: enumerated i
// window: current SV
// delimeter: splitting SV
#define SV_FOR_EACH_SPLIT(index, window, delimeter, sv) \
usize index = 0;\
usize left = (sv)->count + delimeter.count;\
SV window = sv_split_sv(sv, delimeter); \
usize window_len = window.count; \
for (; left > 0; left = left - window_len - delimeter.count, window = sv_split_sv(sv, delimeter), window_len = window.count, index++)

#endif // FC_H

#ifdef FC_IMPLEMENTATION

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

    while (sv->count >= delimeter.count && !sv_eq(sv_from_parts(sv->data, delimeter.count), delimeter)) {
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
        sv->data++;
        sv->count--;
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

#endif // FC_IMPLEMENTATION

// TODO: align all \ at end of line
// TODO: add parenthesis around proc macro arguments
// TODO: make UNREACHABLE accept fmt and args
// TODO: add tests lol