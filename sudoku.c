#include "sudoku.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
	NUM1 =  0b10,
	NUM2 = 0b100,
	NUM3 = 0b1000,
	NUM4 = 0b10000,
	NUM5 = 0b100000,
	NUM6 = 0b1000000,
	NUM7 = 0b10000000,
	NUM8 = 0b100000000,
	NUM9 = 0b1000000000,
} SudokuNumber;

#define has_certain_value(i) ( \
	(i == NUM1) || (i == NUM2) || (i == NUM3) || \
	(i == NUM4) || (i == NUM5) || (i == NUM6) || \
	(i == NUM7) || (i == NUM8) || (i == NUM9) \
)

struct __Sudoku {
	char puzzle[81];
	uint16_t possible[81];
};

static void sudoku_to_context(const char* sudo, Sudoku* ctx) {
	for (int i = 0; i < 81; ++i) {
		ctx->puzzle[i] = sudo[i] - '0';
	}
}
static void context_to_sudoku(const Sudoku* ctx, char* sudo) {
	for (char i = 0; i < 81; ++i) {
		sudo[i] = ctx->puzzle[i] + '0';
	}
}
static int is_done(Sudoku* ctx) {
	for (int i = 0; i < 81; ++i) {
		if (!ctx->puzzle[i]) {
			return 0;
		}
	}
	return 1;
}

static void sudoku_fill_possible(Sudoku* ctx) {
	for (char j = 0; j < 9; ++j) {
		for (char i = 0; i < 9; ++i) {
			char pos = i + j * 9;
			if (ctx->puzzle[pos]) {
				continue;
			}
			uint16_t res = 0xfc01;
			for (char col = pos - i; col < pos - i + 9; ++col) {
				if (ctx->puzzle[col]) {
					res |= 0b1 << ctx->puzzle[col];
				}
			}
			for (char row = i; row < 81; row += 9) {
				if (ctx->puzzle[row]) {
					res |= 0b1 << ctx->puzzle[row];
				}
			}
			char sqr_end = 3 * (i / 3 + (j / 3) * 9) + 21; 
			for (char sqr = sqr_end - 21; sqr < sqr_end; ++sqr) {
				if (ctx->puzzle[sqr]) {
					res |= 0b1 << ctx->puzzle[sqr];
				}
				if (sqr % 3 == 2) {
					sqr += 6;
				}
			}
			ctx->possible[pos] = ~(res | 0xfc01);
		}
	}
}
#define set_if_val(ctx, pos, val) do { \
	uint16_t cmp = (val); \
	if (!((ctx)->possible[pos] & cmp)) break; \
	switch (cmp) { \
	case NUM1: (ctx)->puzzle[pos] = 1; (ctx)->possible[pos] = 0; break; \
	case NUM2: (ctx)->puzzle[pos] = 2; (ctx)->possible[pos] = 0; break; \
	case NUM3: (ctx)->puzzle[pos] = 3; (ctx)->possible[pos] = 0; break; \
	case NUM4: (ctx)->puzzle[pos] = 4; (ctx)->possible[pos] = 0; break; \
	case NUM5: (ctx)->puzzle[pos] = 5; (ctx)->possible[pos] = 0; break; \
	case NUM6: (ctx)->puzzle[pos] = 6; (ctx)->possible[pos] = 0; break; \
	case NUM7: (ctx)->puzzle[pos] = 7; (ctx)->possible[pos] = 0; break; \
	case NUM8: (ctx)->puzzle[pos] = 8; (ctx)->possible[pos] = 0; break; \
	case NUM9: (ctx)->puzzle[pos] = 9; (ctx)->possible[pos] = 0; break; \
	} \
} while (0)
static void sudoku_set_possible_one(Sudoku* ctx, char i, char j) {
	char pos = i + j * 9;
	set_if_val(ctx, pos, ctx->possible[pos]);
	if (ctx->puzzle[pos]) return;
	uint16_t res = 0xfc01;
	for (char col = pos - i; col < pos - i + 9; ++col) {
		res |= ctx->possible[col] | (0b1 << ctx->puzzle[col]);
	}
	set_if_val(ctx, pos, ~res);
	if (ctx->puzzle[pos]) return;
	res = 0xfc01;
	for (char row = i; row < 81; row += 9) {
		res |= ctx->possible[row] | (0b1 << ctx->puzzle[row]);
	}
	set_if_val(ctx, pos, ~res);
	if (ctx->puzzle[pos]) return;
	res = 0xfc01;
	char sqr_end = 3 * (i / 3 + (j / 3) * 9) + 21; 
	for (char sqr = sqr_end - 21; sqr < sqr_end; ++sqr) {
		res |= ctx->possible[sqr] | (0b1 << ctx->puzzle[sqr]);
		if (sqr % 3 == 2) {
			sqr += 6;
		}
	}
	set_if_val(ctx, pos, ~res);
}
static void sudoku_set_possible(Sudoku* ctx) {
	for (char j = 0; j < 9; ++j) {
		for (char i = 0; i < 9; ++i) {
			if (!ctx->puzzle[i + j * 9]) {
				sudoku_set_possible_one(ctx, i, j);
			}
		}
	}
	return;
}

Sudoku* sudoku_alloc() {
	return malloc(sizeof(Sudoku));
}
void sudoku_free(Sudoku* ctx) {
	if (ctx) {
		free(ctx);
	}
}
int sudoku_solve(const char* sudoku_line, Sudoku* ctx) {
	Sudoku other_ctx = { 0 };
	if (!ctx) {
		ctx = &other_ctx;
	}
	sudoku_to_context(sudoku_line, ctx);
	int counter = 0;
	for (; !is_done(ctx) && counter < 256; ++counter) {
		sudoku_fill_possible(ctx);
		sudoku_set_possible(ctx);
	}
#ifdef SUDOKU_LOG
	char buffer[81] = { 0 };
	context_to_sudoku(ctx, buffer);
	sudoku_print(buffer);
#endif // SUDOKU_LOG
	return counter;
}
int sudoku_check(const Sudoku* ctx, const char* sudo) {
	for (char i = 0; i < 81; ++i) {
		if (ctx->puzzle[i] != sudo[i] - '0') {
#ifdef SUDOKU_LOG
			char buffer[81] = { 0 };
			context_to_sudoku(ctx, buffer);
			sudoku_print(buffer);
#endif // SUDOKU_LOG
			return 0;
		}
	}
	return 1;
}
#ifdef SUDOKU_LOG
void sudoku_print(const char* sudo) {
	char* ptr = (char*)sudo;
	printf("+-------+-------+-------+\n");
	for (int j = 0; j < 9; ++j) {
		printf("|");
		for (int i = 0; i < 9; ++i) {
			printf(" %c", *ptr);
			++ptr;
			if (i % 3 == 2) printf(" |");
		}
		printf("\n");
		if (j % 3 == 2) printf("+-------+-------+-------+\n");
	}
}
#endif

void sudoku_solve_file(const char* file_path, Sudoku* sudo, int count_to_solve) {
	FILE* file = fopen(file_path, "rb");
	if (!file) {
		return;
	}

	Sudoku ctx = { 0 };
	if (!sudo) {
		sudo = &ctx;
	}

	fseek(file, 16, SEEK_CUR);
	for (int count = 0; count < count_to_solve; ++count) {
		char buffer[81] = { 0 };
		if (fread(buffer, sizeof(char), 81, file) != 81) break;
		fseek(file, 1, SEEK_CUR);
		
		int steps = sudoku_solve(buffer, sudo);
		char solution[81] = { 0 };
		if (fread(solution, sizeof(char), 81, file) != 81) break;
		if (sudoku_check(sudo, solution)) {
			printf("%-3d Correct solution in %d steps!\n", count + 1, steps);
		} else {
			printf("%-3d Incorrect solution :(\n", count + 1);
		}
		fseek(file, 1, SEEK_CUR);
	}
	fclose(file);
}
void sudoku_print_solution(const Sudoku* sudo) {
	char buffer[81] = { 0 };
	context_to_sudoku(sudo, buffer);
	printf("%.*s\n", 81, buffer);
}


