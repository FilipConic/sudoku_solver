#ifndef SUDOKU_SOLVE_H
#define SUDOKU_SOLVE_H

typedef struct __Sudoku Sudoku;

Sudoku* sudoku_alloc();
void sudoku_free(Sudoku* sudo);

int sudoku_solve(const char* buffer, Sudoku* sudo);
int sudoku_check(const Sudoku* sudo, const char* solution);

#ifdef SUDOKU_LOG
void sudoku_print(const char* sudo);
#endif // SUDOKU_LOG

void sudoku_solve_file(const char* file_path, int count_to_solve);
int sudoku_test_file(const char* file_path, Sudoku* sudo, int count_to_solve);
void sudoku_print_solution(const Sudoku* sudo);

#endif // SUDOKU_SOLVE_H
