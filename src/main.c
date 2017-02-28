#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sudoku.h"

void print_help(void) {
    printf("sudokist:\n    command line utility to solve 9x9 sudokus.\n\n");
    printf("usage: sudokist [options] input output.\n");
    printf("arugments:\n");
    printf("    input           path pointing to sudoku table input file.\n");
    printf("    output          path pointing to sudoku table output file.\n");
    printf("options:\n");
    printf("    --help, -h      print this help message.\n");
    printf("    --verbose, -v   increase verbosity level.\n");
}

char *get_path(char *path) {
    if (path[0] != '/' || path[0] != '\\') {
        char *prefix = "./";
        size_t prefix_len = strlen(prefix);
        size_t path_len = strlen(path);
        size_t final_len = path_len + prefix_len;
        char *temp = malloc(final_len + 1);

        memcpy(temp, prefix, prefix_len);
        memcpy(temp + prefix_len, path, path_len);
        temp[final_len] = '\0';
        return temp;
    }
    return path;
}

int main(int argc, char *argv[]) {
    bool verbose = false;
    bool help = false;
    // Figure out if we're expecting arguments.
    bool processing_args = false;
    int argi = 0;
    char *paths[2];

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        // If an argument starts by '-' it must be an option otherwise
        // it must be an argument.
        if (arg[0] == '-') {
            if (processing_args) {
                fprintf(stderr, "error: options should be added before arguments.\n");
                return 1;
            }

            // If the first 2 characters starts by '-' it must be a complete
            // option name otherwise, an alias one.
            if (arg[1] == '-') {
                arg += 2;

                if (strcmp(arg, "verbose") == 0) {
                    verbose = true;
                } else if (strcmp(arg, "help") == 0) {
                    help = true;
                } else {
                    fprintf(stderr, "error: unknown option '%s'.\n", arg);
                    return 1;
                }
            } else {
                arg += 1;

                if (strcmp(arg, "v") == 0) {
                    verbose = true;
                } else if (strcmp(arg, "h") == 0) {
                    help = true;
                } else {
                    fprintf(stderr, "error: unknown option alias '%s'.\n", arg);
                    return 1;
                }
            }
        } else {
            processing_args = true;
            paths[argi] = arg;
            // Check if we've exceeded the number of required arguments.
            if (++argi > 2) {
                fprintf(stderr, "error: unknown extra argument '%s'.\n", arg);
                return 1;
            }
        }
    }

    // If no arguments(after processed) have been passed
    // and help flag was set we print help.
    if (argi == 0 && help) {
        print_help();
        return 0;
    }

    if (argi < 1) {
        fprintf(stderr, "error: 'input' and 'output' arguments are required.\n");
        print_help();
        return 1;
    }

    char *input = get_path(paths[0]);
    char *output = get_path(paths[1]);

    if (verbose) {
        printf(" loading table at %s\n", input);
    }

    struct sudoku *s = sudoku_load(input);
    if (s == NULL) {
        fprintf(stderr, "error: unable to load sudoku.\n");
        return 1;
    }

    if (sudoku_solve(s) < 0) {
        fprintf(stderr, "error: unable to solve sudoku.\n");
        return 1;
    }

    if (sudoku_save(output, s) < 0) {
        fprintf(stderr, "error: unable to save sudoku.\n");
        return 1;
    }
    return 0;
}
