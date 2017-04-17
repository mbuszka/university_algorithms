#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_N 1000000
#define check(_prev, _col) (next[_prev][_col])


short combinations[512];
// char next[64][8];
long score[2][64];
char backtrack[MAX_N][64];
char board[MAX_N][5];
char popcnt[8] = {0, 1, 1, 2, 1, 2, 2, 3};
int  one_col_combinations;


int build_combinations() {
  int count = 0;
  for (int i=0; i<512; i++) {
    if (!(
      ((i & 0x100) && (i & 0xA))  ||
      ((i & 0x80)  && (i & 0x5))  ||
      ((i & 0x40)  && (i & 0x22)) ||
      ((i & 0x20)  && (i & 0x41)) ||
      ((i & 0x8)   && (i & 0x104))
    )) combinations[count++] = i;
    if (i == 63) one_col_combinations = count;
  }
  return count;
}

// void build_next() {
//   for (int prev = 0; prev<64; prev++) {
//     if (((prev & 0x20) && (prev & 1)) || ((prev & 0x4) && (prev & 0x8)))
//       continue;
//     for (int col=0; col<8; col++) {
//       next[prev][col] = !(((prev & 4) && (col & 1)) ||
//                           ((prev & 1) && (col & 4)) ||
//                           ((prev & 0x28) && (col & 2)) ||
//                           ((prev & 0x10) && (col & 5)));
//     }
//   }
// }


void inline one_step(short combination, int mask, int iter) {
  int newcol = combination & 7;
  if ((newcol & mask) == newcol) {
    int old_double = combination >> 3;
    int new_double = combination & 0x3F;
    int even = iter & 1;
    long new_score = score[!even][old_double] + popcnt[newcol];
    if (new_score > score[even][new_double]) {
      score[even][new_double] = new_score;
      backtrack[iter][new_double] = old_double;
    }
  }
}

char inline read_mask(int i) {
  fgets(&board[i][0], 5, stdin);
  char mask = 0;
  mask = ((board[i][0] == '.') << 2) | ((board[i][1] == '.') << 1) | ((board[i][2] == '.'));
  return mask;
}

void print_score(int col) {
  printf("[");
  for (int i=0; i<64; i++) {
    printf("%ld ", score[col][i]);
  }
  printf("]\n");
}

int main() {
  bzero(&score, sizeof(score));
  // bzero(&next, sizeof(next));
  int combination_count = build_combinations();
  int n;
  char mask;
  char inbuf[20];
  fgets(inbuf, 20, stdin);
  sscanf(inbuf, "%d", &n);

  mask = read_mask(0);
  // printf("%x\n", mask);
  for (int i=0; i<8; i++) {
    one_step(i, mask, 0);
  }
  // print_score(0);
  if (n == 1) goto fin;

  mask = read_mask(1);
  // printf("%x\n", mask);
  for (int i=0; i<one_col_combinations; i++) {
    one_step(combinations[i], mask, 1);
  }
  // print_score(1);
  if (n == 2) goto fin;

  for (int i=2; i<n; i++) {
    bzero(&score[i&1][0], 64 * sizeof(long));
    mask = read_mask(i);

    for (int j=0; j<combination_count; j++) {
      one_step(combinations[j], mask, i);
    }
    // print_score(i&1);
  }
fin:
  long max_score = 0;
  int  max_idx = 0;
  for (int i=0; i<64; i++) {
    if (max_score < score[1-(n&1)][i]) {
      max_idx = i;
      max_score = score[1-(n&1)][i];
    }
  }
  printf("%ld\n", max_score);
  fflush(stdout);
  for (int i=n-1; i>=0; i--) {
    int col = max_idx & 7;
    for (int j=0; j<3; j++) {
      board[i][j] = col & (1 << (2 - j)) ? 'S' : board[i][j];
    }
    max_idx = backtrack[i][max_idx];
  }
  for (int i=0; i<n; i++) {
    printf("%.3s\n", &board[i][0]);
  }

}
