#ifndef SUDOKU_H
#define SUDOKU_H

const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

extern int neighbors[N][NEIGHBOR];//81个数 20个邻居.
extern int board[30][N];//结果
extern int spaces[30][N];
extern int nspaces[30];
extern int (*chess)[COL];

void init_neighbors();
void input(const int i,const char in[N]);
void init_cache();

bool available(int i,int guess, int cell);

bool solve_sudoku_basic(int i, int which_space);
// bool solve_sudoku_min_arity(int which_space);
// bool solve_sudoku_min_arity_cache(int which_space);
// bool solve_sudoku_dancing_links(int unused);
bool solved(int i);
#endif
