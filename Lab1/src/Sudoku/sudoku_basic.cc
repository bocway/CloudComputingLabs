#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <queue>
#include <queue>
#include <pthread.h>
#include <semaphore.h> 
#include "sudoku.h"
using namespace std;

// int board[N];
// int spaces[N];
// int nspaces;
//int (*chess)[COL] = (int (*)[COL])board;

queue<boardStruct> q;
map<long int,outStruct> out;
sem_t out_full; 
sem_t out_empty; 
pthread_mutex_t out_mutex;

sem_t in_full; 
sem_t in_empty; 
pthread_mutex_t in_mutex;
long int job_id=0;
static void find_spaces(boardStruct &b)
{
  b.nspaces = 0;
  for (int cell = 0; cell < N; ++cell) {
    if (b.board[cell] == 0)
      b.spaces[b.nspaces++] = cell;
  }
}

void input(const char in[N])
{
  boardStruct b;
  for (int cell = 0; cell < N; ++cell) {
    b.board[cell] = in[cell] - '0';
    assert(0 <= b.board[cell] && b.board[cell] <= NUM);
  } 
  find_spaces(b);
  b.id=job_id;
  job_id++;
  b.finish=false;

  sem_wait(&in_empty); 
  pthread_mutex_lock(&in_mutex);
  q.push(b);
  printf("id %d has been append\n",b.id);
  pthread_mutex_unlock(&in_mutex);
  sem_post(&in_full); 
  


  
}

// bool available(int guess, int cell)
// {
//   for (int i = 0; i < NEIGHBOR; ++i) {
//     int neighbor = neighbors[cell][i];
//     if (board[neighbor] == guess) {
//       return false;
//     }
//   }
//   return true;
// }
