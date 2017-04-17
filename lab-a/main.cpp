#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define MAX_N 2002
#define MAX_M 2002
#define PUSH(_x, _y) do {*(x_top++) = _x; *(y_top++) = _y; } while(0)

char board[MAX_N][MAX_M]; 
int16_t stack_x[MAX_N * MAX_M];
int16_t stack_y[MAX_N * MAX_M];
int16_t *x_top = stack_x;
int16_t *y_top = stack_y;

char conn_l[6] = {0, 1, 1, 0, 0, 1};
char conn_r[6] = {0, 0, 0, 1, 1, 1};
char conn_u[6] = {0, 0, 1, 1, 0, 1};
char conn_d[6] = {0, 1, 0, 0, 1, 1};

void dfs(int16_t startx, int16_t starty) {
  int16_t x;   
  int16_t y;
  PUSH(startx, starty);
  do {
    x = *(--x_top);
    y = *(--y_top);
    switch (board[y][x]) {
      case 'a': break;
      case 'A': break;
      case 'B': if (conn_r[board[y][x-1] - 'A']) PUSH(x-1, y);
		if (conn_u[board[y+1][x] - 'A']) PUSH(x, y+1);
		break;
      case 'C': if (conn_r[board[y][x-1] - 'A']) PUSH(x-1, y);
		if (conn_d[board[y-1][x] - 'A']) PUSH(x, y-1);
 		break;
      case 'D': if (conn_l[board[y][x+1] - 'A']) PUSH(x+1, y);
		if (conn_d[board[y-1][x] - 'A']) PUSH(x, y-1);
      		break;
      case 'E': if (conn_l[board[y][x+1] - 'A']) PUSH(x+1, y);
		if (conn_u[board[y+1][x] - 'A']) PUSH(x, y+1);
		break;
      case 'F': if (conn_u[board[y+1][x] - 'A']) PUSH(x, y+1);
		if (conn_d[board[y-1][x] - 'A']) PUSH(x, y-1);
		if (conn_l[board[y][x+1] - 'A']) PUSH(x+1, y);
		if (conn_r[board[y][x-1] - 'A']) PUSH(x-1, y);
		break;
    }
    board[y][x] = 'A';
  } while(x_top > stack_x);
}

int main() {
  int n, m;
  int count = 0;
  scanf("%d %d", &n, &m);
  
  memset(board, 'A', MAX_N * MAX_M);
 
  for (int i=1; i<=n; i++) {
    scanf("%2000s\n", &board[i][1]);
    board[i][m+1] = 'A';
  }
  /*
  for (int i=1; i<=n; i++) {
    write(1, &board[i][1], m);
    write(1, "\n", 1);
  }
  */
  for (int i=1; i<=n; i++) {
    for (int j=1; j<=m; j++) {
      if (board[i][j] != 'A' && board[i][j] != 'a') {
	dfs(j, i);
	/*for (int i=1; i<=n; i++) {
    	  write(1, &board[i][1], m);
    	  write(1, "\n", 1);
  	}
	write(1, "\n\n", 2);*/
	count++;
      }  
    }
  }
  printf("%d\n", count);
}


