#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sudoku.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: provide a flag!\nexample: './sudo_solve -h'\n");
		return 1;
	}
	
	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
		printf("work in progress!");
	} else if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "--file")) {
		if (argc < 3) {
			fprintf(stderr,
		   		"ERROR: for the 'file' flag you need to provide one more argument, a .csv file with sudoku puzzles!\n"
		   		"example: ./sudo_solve -f <input>.csv\n"
		   	);
			return 1;
		}
		int count = (argc == 4 ? atoi(argv[3]) : 256);
		sudoku_solve_file(argv[2], count);
	} else if (!strcmp(argv[1], "-s") || !strcmp(argv[1], "--solve")) {
		if (argc < 3) {
			fprintf(stderr, 
		   		"ERROR: for the 'solve' flag you need to provide one more argument, a sudoku puzzle of 81 bytes!\n"
		   		"example: ./sudo_solve -s <sudoku>\n"
		   	);
			return 1;
		}
		Sudoku* sudoku = sudoku_alloc();
		sudoku_solve(argv[2], sudoku);
		#ifndef SUDOKU_LOG
		sudoku_print_solution(sudoku);
		#endif
		sudoku_free(sudoku);
	} else if (!strcmp(argv[1], "-t") || !strcmp(argv[1], "--test")) {
		if (argc < 3) {
			fprintf(stderr, 
		   		"ERROR: for the 'test' flag you need to provide a .csv file with sudoku puzzles and their solutions!\n"
		   		"example: ./sudo_solve -t <input>.csv [count of test]\n"
		   	);
			return 1;
		}
		int count = (argc == 4 ? atoi(argv[3]) : 256);
		int correct_count = sudoku_test_file(argv[2], NULL, count);
		printf("count of correctly solved sudoku puzzles is %d out of %d, which is %3.2lf%% correct\n", correct_count, count, (double)correct_count / count * 100.0);
	}
	return 0;
}
