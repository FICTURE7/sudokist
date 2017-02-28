#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/**
	@brief Height of a sudoku table.
*/
#define SUDOKU_TABLE_HEIGHT 9

/**
	@brief Width of a sudoku table.
*/
#define SUDOKU_TABLE_WIDTH 9

/**
	@breif Value of an unknown in a sudoku table.
*/
#define SUDOKU_UNKNOWN -1

/**
	@brief Gets the value of a cell in the specified sudoku struct at the specified x and y coordinates.

	@param s Pointer to sudoku struct.
	@param x x coordinate.
	@param y y coordinate.
	@return Returns value of cell.
*/
#define SUDOKU_GET(s, x, y) s->table[(y * SUDOKU_TABLE_WIDTH) + x]

/**
	@brief Sets the value of a cell in the specified sudoku struct at the specified x and y coordinates.

	@param s Pointer to sudoku struct.
	@param x x coordinate.
	@param y y coordinate.
	@param v New value.
*/
#define SUDOKU_SET(s, x, y, v) s->table[(y* SUDOKU_TABLE_WIDTH) + x] = v

/**
	@brief Simple structure representing a sudoku table.

	Represents a Soduko table.
*/
struct sudoku {
	/**
		@brief Table containing the values of the sudoku.

		Table containing the values of the sudoku and is of size
		SODUKO_TABLE_HEIGHT * SODUKO_TABLE_WIDTH.
	*/
	char table[SUDOKU_TABLE_HEIGHT * SUDOKU_TABLE_WIDTH];
};

/**
	@brief Loads the Soduko table file at the specified path.

	Loads and parses the Soduko table file at the specified path and returns
	a sudoku struct if success otherwise it returns NULL.

	@param path Path pointing to the soduko table file.
	@return Returns a pointer to a sudoku struct if success otherwise it returns NULL.
*/
struct sudoku *sudoku_load(const char *path);

/**
	@brief Saves the specified sudoku struct at the specified path.

	Saves the specified sudoku struct at the specified and returns 0
	if success otherwise -1.

	@param path Path pointing to where to save the Soduko table file.
	@param sudoku Pointer to the sudoku struct to be saved.
	@return Returns 0 if success otherwise -1.
*/
int sudoku_save(const char *path, struct sudoku *sudoku);

/**
	@brief Solves the specified sudoku struct.

	Solves the specified sudoku struct using a backtracking algorithm.

	@param sudoku Pointer to the sudoku struct to be solved.
	@return Returns 0 if success otherwise -1.
*/
int sudoku_solve(struct sudoku *sudoku);
#endif /* SUDOKU_H */
