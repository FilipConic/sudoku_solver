# Sudoku Solver

This is a simple sudoku solver. Doesn't work perfectly yet (need to add guessing and backtracking). Other than that to use it compile it with GNU-make by running ```make``` and then just run ```./sudo_solve``` with a chosen flag. Whenever you are providing the program a sudoku puzzle it needs to be a string of 81 bytes, where 0 represents an empty space and 1-9 filled in spaces.

| Flags | |
|:--|:--|
| -h/--help | prints this same table to the screen|
| -s/--solve ```sudoku``` | solve an 81 string that represents a sudoku puzzle |
| -f/--file ```sudoku.csv``` | solve a file full of sudoku puzzles |


