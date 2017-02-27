#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main(int argc, char *argv[]) {
	//TODO: Command line handling.
	
	struct sudoku *s = sudoku_load("./test.txt");
	if (s == NULL) {
		fprintf(stderr, " unable to load sudoku file.\n");
		return 1;
	}

	// Solve the soduko.
	if (sudoku_solve(s) < 0) {
		fprintf(stderr, " unable to solve sudoku.\n");
	}

	// Save the soduko.
	if (sudoku_save("./test_solved.txt", s) < 0) {
		fprintf(stderr, " unable to save sudoku.\n");
	}

	free(s);
	return 0;
}
