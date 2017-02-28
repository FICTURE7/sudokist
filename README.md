# Sudokist
A simple program written in C to solve 9 by 9 Sudokus using a backtraking
algorithm.

### Building
Just use the `build.sh` script.

```bash
./build.sh "MSYS Makefiles" "Release"
```

### Using
It works by making use of table files.

#### Table format
The table format is very simple, uassigned/unknowns are represented using 'x'. Make sure not to include white space at the end
of each line and a line ending at the end of the table. Here is an example.
```
1 x x 5 7 x x 3 8
7 x x x x 8 4 x x
x 8 3 x 1 x 5 x x
x 1 x 8 x 3 x x 2
8 x 7 x x x 6 x 4
5 x x 7 x 6 x 8 x
x x 8 x 9 x 7 2 x
x x 5 3 x x x x 1
9 7 x x 8 5 x x 6
```
 
Save the table as `table.txt` and run the following command.
```bash
sudokist table.txt table-solved.txt
```

A file named `table-solved.txt` should be created with the solution.