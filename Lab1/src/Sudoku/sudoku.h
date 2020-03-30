#ifndef SUDOKU_H
#define SUDOKU_H
#include<queue>
#include <pthread.h>
#include <semaphore.h> 
#include <map>
using namespace std;
const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
extern sem_t out_full; 
extern sem_t out_empty; 
extern pthread_mutex_t out_mutex;

extern sem_t in_full; 
extern sem_t in_empty; 
extern pthread_mutex_t in_mutex;

typedef struct {
  int id;
  int board[N];
  int spaces[N];
  int nspaces;
  bool finish;
} boardStruct;

typedef struct {
  int id;
  int board[N];
  bool finish;
} outStruct;

const int NUM = 9;

// extern int neighbors[N][NEIGHBOR];//81个数 20个邻居.
// extern int board[N];//结果
// extern int spaces[N];
// extern int nspaces;
//extern int (*chess)[COL];
extern queue<boardStruct> q;
extern map<long int,outStruct>  out;

void init_neighbors();
void input(const char in[N]);
void init_cache();

bool available(int i,int guess, int cell);

// bool solve_sudoku_basic(int i, int which_space);
// bool solve_sudoku_min_arity(int which_space);
// bool solve_sudoku_min_arity_cache(int which_space);
bool solve_sudoku_dancing_links(boardStruct boar);
// bool solved();
#endif
