#ifndef SUDOKU_H
#define SUDOKU_H

const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;


extern int neighbors[N][NEIGHBOR];
extern int board[30][N];
extern int spaces[30][N];
extern int nspaces[30];
extern int (*chess)[COL];

void init_neighbors();
void input(const int i, const char in[N]);
void init_cache();

bool available(int i, int guess, int cell);

bool solve_sudoku_basic(int i, int which_space);
bool solved(int i);
#endif
