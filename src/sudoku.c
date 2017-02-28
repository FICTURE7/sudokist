#include "sudoku.h"

#define VERBOSE

struct sudoku *sudoku_load(const char *path) {
	assert(path != NULL);

	struct sudoku *ret = NULL;
	FILE *file = fopen(path, "rt");
	if (file == NULL) {
#ifdef VERBOSE
		fprintf(stderr, " unable to load, fopen failed.\n");
#endif
		goto out;
	}

	ret = calloc(1, sizeof(struct sudoku));
	if (ret == NULL) {
#ifdef VERBOSE
		fprintf(stderr, " unable to load, calloc failed.\n");
#endif
		fclose(file);
		goto out;
	}

	size_t line_size = SUDOKU_TABLE_WIDTH * 4;
	char *line = NULL;
	// Buffer which is going to contain the line read
	// by fgets.
	char *buffer = malloc(sizeof(char) * line_size);
	if (buffer == NULL) {
#ifdef VERBOSE
		fprintf(stderr, " unable to load, malloc failed.\n");
#endif
		goto out_close_free_null;
	}

	int y = 0;

	while (true) {
		line = fgets(buffer, line_size, file);
		if (line == NULL) {
			break;
		}
		// Figure whether the current character should be a white space.
		// First byte in buffer must be a not be a white space.
		bool is_space = false;
		size_t len = strlen(line);
		int x = 0;

		// Check if we've got the line completely and
		// subtract 1 so that we ignore the line ending
        // when we're going to loop through its characters.
		if (line[len--] != '\n') {
#ifdef VERBOSE
			fprintf(stderr, " unable to load, could not get line completely from file.\n");
#endif
			goto out_close_free_null;
		}

		for (int i = 0; i < len; i++) {
			char c = line[i];

			if (is_space) {
				if (c == ' ') {
					// Ignore white space and mark
					// the next byte as a character.
					is_space = false;
					continue;
				} else {
#ifdef VERBOSE
					fprintf(stderr, " unable to load, syntax err expected whitespace. ln:%d col:%d.\n", y + 1, i + 1);
#endif
					goto out_close_free_null;
				}
			} else {
				if (c == ' ') {
#ifdef VERBOSE
					fprintf(stderr, " unable to load, syntax err unexpected whitespace. ln:%d col:%d.\n", y + 1, i + 1);
#endif
					goto out_close_free_null;
				}
			}

			char val = 0;
			if (c == 'x') {
				// 'x' represents an unknown.
				val = SUDOKU_UNKNOWN;
			} else if (c >= 49 && c <= 57) {
				// Figure out if its an integer character between 1-9 and
				// convert it to a proper integer.
				val = c - 48;
			} else {
				// Otherwise syntax error.
#ifdef VERBOSE
				fprintf(stderr, " unable to load, syntax err expected number between 1-9 or x for unknowns. ln:%d col:%d.\n", y + 1, i + 1);
#endif
				goto out_close_free_null;
			}

			SUDOKU_SET(ret, x, y, val);
			// Mark next byte as space.
			is_space = true;
			// Move pointer to figure out the width
			// of the table.
			x++;
		}

		if (x < SUDOKU_TABLE_WIDTH) {
#ifdef VERBOSE
			fprintf(stderr, " unable to load, table width must be %d but was %d at ln: %d.\n", SUDOKU_TABLE_WIDTH, x, y + 1);
#endif
			goto out_close_free_null;
		}
		// Move pointer on y-axis.
		y++;
	}

	if (y < SUDOKU_TABLE_HEIGHT) {
#ifdef VERBOSE
		fprintf(stderr, " unable to load, table height must be %d but was %d\n", SUDOKU_TABLE_HEIGHT, y);
#endif
		goto out_close_free_null;
	}

	fclose(file);
	goto out;

	// Closes file, frees ret and buffer, and sets ret to NULL.
out_close_free_null:
	fclose(file);
    free(buffer);
	free(ret);
	ret = NULL;
out:
	return ret;
}

int sudoku_save(const char *path, struct sudoku *sudoku) {
	assert(path != NULL);
	assert(sudoku != NULL);

	FILE *file = fopen(path, "w");
	if (file == NULL) {
#ifdef VERBOSE
		fprintf(stderr, " unable to save, fopen failed.\n");
#endif
		return -1;
	}

	//TODO: Check for fwrite failures.

	for (int y = 0; y < SUDOKU_TABLE_HEIGHT; y++) {
		for (int x = 0; x < SUDOKU_TABLE_WIDTH; x++) {
			// Value from the table.
			char tval = SUDOKU_GET(sudoku, x, y);
			// Value to write into the file.
			char wval = 0;

			// If the value from the table was -1, it means
			// its an unknown, otherwise it must be an integer
			// so we add 48 to turn it into an integer character.
			if (tval == SUDOKU_UNKNOWN) {
				wval = 'x';
			} else {
				wval = tval + 48;
			}

			fwrite(&wval, sizeof(char), 1, file);
			// No need to write a space for the last value.
			if (x < SUDOKU_TABLE_HEIGHT - 1) {
				fwrite(" ", sizeof(char), 1, file);
			}
		}

		fwrite("\n", sizeof(char), 1, file);
	}
	fclose(file);
	return 0;
}

// Check the uniqueness of values in the specified x and y rows and column as well
// as its subtable.
static bool sudoku_check(struct sudoku *sudoku, int x, int y) {
	char checked_vals[9];
	int checked_ptr = 0;

	// Check the vertical column.
	for (int i = 0; i < SUDOKU_TABLE_HEIGHT; i++) {
		char tval = SUDOKU_GET(sudoku, x, i);
		if (tval == SUDOKU_UNKNOWN) {
			continue;
		}

		for (int k = 0; k < checked_ptr; k++) {
			if (checked_vals[k] == tval) {
				return false;
			}
		}

		checked_vals[checked_ptr++] = tval;
	}

	checked_ptr = 0;

	// Check horizontal row.
	for (int i = 0; i < SUDOKU_TABLE_WIDTH; i++) {
		char tval = SUDOKU_GET(sudoku, i, y);
		if (tval == SUDOKU_UNKNOWN) {
			continue;
		}

		for (int k = 0; k < checked_ptr; k++) {
			if (checked_vals[k] == tval) {
				return false;
			}
		}

		checked_vals[checked_ptr++] = tval;
	}

	checked_ptr = 0;

	//TODO: Improve the maths maybe XD?
	// Check subtable.
	int subtable_x = x / 3;
	int subtable_y = y / 3;
	for (int j = 0; j < SUDOKU_TABLE_HEIGHT / 3; j++) {
		for (int i = 0; i < SUDOKU_TABLE_WIDTH / 3; i++) {
			// Calculate position relative to main table.
			int actual_x = i + (subtable_x * 3);
			int actual_y = j + (subtable_y * 3);

			char tval = SUDOKU_GET(sudoku, actual_x, actual_y);
			if (tval == SUDOKU_UNKNOWN) {
				continue;
			}

			for (int k = 0; k < checked_ptr; k++) {
				if (checked_vals[k] == tval) {
					return false;
				}
			}

			checked_vals[checked_ptr++] = tval;
		}
	}

	return true;
}

static bool sudoku_backtrack(struct sudoku *sudoku, int x, int y) {
	char tval = SUDOKU_GET(sudoku, x, y);

	// If the value is not an unknown we look for one.
	if (tval != SUDOKU_UNKNOWN) {
		for (y = 0; y < SUDOKU_TABLE_HEIGHT; y++) {
			for (x = 0; x < SUDOKU_TABLE_WIDTH; x++) {
				// If its an unknown we jump out of the loops and backtrack from there.
				if (SUDOKU_GET(sudoku, x, y) == SUDOKU_UNKNOWN) {
					goto found;
				}
			}
		}

		// Return true and jump out of the backtrack node since all cells has been filled.
		return true;
	}

found:
	// Try all 1-9 possible values.
	for (int i = 1; i < 10; i++) {
		SUDOKU_SET(sudoku, x, y, i);
		// Check if the table is correct.
		// If it is, we move down the node.
		if (sudoku_check(sudoku, x, y)) {
			// Check if we're at the end of the line and switch to the next
			// line if needed.
			if (x == SUDOKU_TABLE_WIDTH - 1) {
				if (sudoku_backtrack(sudoku, 0, y + 1))
					return true;
			} else {
				if (sudoku_backtrack(sudoku, x + 1, y))
					return true;
			}
		}
	}

	// No possible solution found on current node, set back to unknown and
	// move up to parent node.
	SUDOKU_SET(sudoku, x, y, SUDOKU_UNKNOWN);
	return false;
}

int sudoku_solve(struct sudoku *sudoku) {
	assert(sudoku != NULL);

	// Start backtracking at 0,0 that is the top left most corner.
	if (sudoku_backtrack(sudoku, 0, 0)) {
		return 0;
	} else {
		return -1;
	}
}
