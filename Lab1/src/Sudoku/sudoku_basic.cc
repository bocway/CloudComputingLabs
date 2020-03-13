#include <assert.h>
#include <stdio.h>

#include <algorithm>

#include "sudoku.h"

int board[30][N];
int spaces[30][N];
int nspaces[30];
int (*chess)[COL] = (int (*)[COL])board;

static void find_spaces(const int i)
{
  nspaces[i] = 0;
  for (int cell = 0; cell < N; ++cell) {
    if (board[i][cell] == 0)
      spaces[i][nspaces[i]++] = cell;
  }
}

void input(const int i,const char in[N])
{
  for (int cell = 0; cell < N; ++cell) {
    board[i][cell] = in[cell] - '0';
    assert(0 <= board[i][cell] && board[i][cell] <= NUM);
  }
	//for(int j = 0; j < N; ++j)
	  //printf("%c",in[j]);
	//printf("\n");  
  find_spaces(i);
}

bool available(int j, int guess, int cell)
{
  for (int i = 0; i < NEIGHBOR; ++i) {
    int neighbor = neighbors[cell][i];
    if (board[j][neighbor] == guess) {
      return false;
    }
  }
  return true;
}

bool solve_sudoku_basic(int i, int which_space)
{
  if (which_space >= nspaces[i]) {
    return true;
  }
  //for(int j = 0; j < N; ++j)
    //printf("%d",board[i][j]);
  //printf(" %d %d   solve_sudoku_basic\n",nspaces[i],i);
  // find_min_arity(which_space);
  int cell = spaces[i][which_space];

  for (int guess = 1; guess <= NUM; ++guess) {
    if (available(i, guess, cell)) {
      // hold
	//printf("%d %d %d\n",i,cell,board[i][cell]);
      assert(board[i][cell] == 0);
      board[i][cell] = guess;

      // try
      if (solve_sudoku_basic(i, which_space+1)) {
	//for(int j = 0; j < N; ++j)
	  //printf("%d",board[i][j]);
	//printf("%d\n",i);
        return true;
      }

      // unhold
      assert(board[i][cell] == guess);
      board[i][cell] = 0;
    }
  }
  return false;
}
