#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include "sudoku.h"
int result[30][N];
int total_solved = 0;
char puzzle_thread[30][128];
int thread_num[30] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}



void* sudoku1(void *i1)
{
  int i = *(int *)i1;
  bool (*solve)(int, int) = solve_sudoku_basic;

  input(i, puzzle_thread[i]);
  if (solve(i, 0)) {
    total_solved++;
    if (!solved(i))
      assert(0);
  }
  
  else {
    printf("No: %s", puzzle_thread[i]);
  }
  //for(int j = 0; j < N; ++j)
    //result[*i-1][j] = board[j];
}



int main(int argc, char* argv[])
{
  init_neighbors();

  FILE* fpp1 = fopen(argv[1], "r");
  char puzzlep[128];
  
  int total = 0;
  //bool (*solve)(int) = solve_sudoku_basic;
  printf("Assuming your program has been inputted with two file names from stdin: \n");
  printf("%s\n",argv[1]);
  printf("\n");
  printf("%s has the following content:\n",argv[1]);
  while (fgets(puzzlep, sizeof puzzlep, fpp1) != NULL) {
    printf("%s", puzzlep);
  }


  int thread_n = 4;
  pthread_t tids[thread_n];
  char puzzle[128];
  int break_flag = 30;


  
  printf("Then your program should output the following results to stdout:\n");
  int64_t start = now();
  FILE* fp = fopen(argv[1], "r");
  //while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
  while(1) {
    for (int i = 0; i < thread_n; ++i)
    {
      if(fgets(puzzle, sizeof puzzle, fp) != NULL && strlen(puzzle) >= N)
      {
	++total;
	for(int j = 0; j < N; ++j)
	  puzzle_thread[i][j] = puzzle[j];
	int ret = pthread_create(&tids[i], NULL, sudoku1, &thread_num[i]);
      }
      else
      {
	break_flag = i;
	break;
      }
    }
    for (int i = 0; i < thread_n && i < break_flag; ++i)
    {
      int status;
      pthread_join(tids[i],(void **)&status);
    }
    for (int i = 0; i < thread_n && i < break_flag; ++i)
    {
      for(int j = 0; j < 81; ++j)
	printf("%d",board[i][j]);
      printf("\n");
    }
    if (break_flag != 30)
      break;






    //if (strlen(puzzle[0]) >= N) {
      //++total;
      //input(puzzle[0]);
      //init_cache();
      //if (solve_sudoku_min_arity_cache(0)) {
      //if (solve_sudoku_min_arity(0))
      //if (solve_sudoku_basic(0)) {
      //if (solve(0)) {
        //++total_solved;
	//for(int i = 0; i < 81; ++i)
	  //printf("%d",board[i]);
	//printf("\n");
        //if (!solved())
          //assert(0);
      //}
      //else {
        //printf("No: %s", puzzle[0]);
      //}
    //}





  }
  int64_t end = now();
  FILE* fpp2 = fopen(argv[1], "r");
  int total1 = 1;
  while (fgets(puzzlep, sizeof puzzlep, fpp2) != NULL) {
    if(total1 == 1)
      printf("Where the %dst line in the output is the solution to puzzle\n",total1++);
    else if(total1 == 2)
      printf("Where the %dnd line in the output is the solution to puzzle\n",total1++);
    else if(total1 == 3)
      printf("Where the %drd line in the output is the solution to puzzle\n",total1++);
    else
      printf("Where the %dth line in the output is the solution to puzzle\n",total1++);
    printf("%s", puzzlep);
  }
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);

  return 0;
}

