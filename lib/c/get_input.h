#ifndef GET_INPUT_H
#define GET_INPUT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* get_input(int argc, char **argv);

#endif // GET_INPUT_H

#ifdef GET_INPUT_IMPLEMENTATION

char* get_input(int argc, char **argv) {
    char *filename = "../input.txt";

    if (argc > 1) {
        filename = argv[1];
    }

    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        printf("Error opening file %s\n", filename);
        exit(0);
    }

    fseek(f, 0, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *input = malloc(fsize + 1);
    fread(input, fsize, 1, f);
    fclose(f);

    input[fsize] = 0;

    return input;
}

#endif // GET_INPUT_IMPLEMENTATION