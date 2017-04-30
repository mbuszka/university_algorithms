#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdint.h>

char word[1001];

uint16_t  P[1000][1000];
uint16_t  nterm[26];
uint16_t  prods[0x10000];

void read_productions(int nt_cnt, int t_cnt) {
  bzero(prods, sizeof(prods));
  bzero(nterm, sizeof(nterm));

  for (int k=0; k<nt_cnt; k++) {
    char t, l, r;
    scanf(" %c %c %c", &t, &l, &r);
    int i = l - 'A';
    int j = r - 'A';
    uint16_t prod = 1 << (t - 'A');
    uint16_t mask = (1 << j) | (1 << (i + 8));
    for (int m=0; m<0x10000; m++) {
      prods[m | mask] |= prod;
    }
  }

  for (int i=0; i<t_cnt; i++) {
    char n, t;
    scanf(" %c %c ", &n, &t);
    nterm[t - 'a'] |= 1 << (n - 'A');
  }
}


int cyk(int word_len) {
  bzero(P, sizeof(P));

  for (int s=0; s < word_len; s++) {
    P[0][s] |= nterm[word[s] - 'a'];
  }

  for (int l=2; l<=word_len; l++)
    for (int s=0; s<=word_len-l; s++)
      for (int p=1; p<l; p++)
        P[l-1][s] |= prods[ (P[p-1][s] << 8) | P[l-p-1][s+p]];

  return P[word_len-1][0] & 1;
}



void run_instance() {
  int terminal_prod_cnt, nonterminal_prod_cnt, res, word_len;

  scanf("%d %d", &nonterminal_prod_cnt, &terminal_prod_cnt);
  read_productions(nonterminal_prod_cnt, terminal_prod_cnt);

  scanf("%1000s", word);
  word_len = strlen(word);

  res = cyk(word_len);
  printf("%s\n", res ? "TAK" : "NIE");
}

int main() {
  int instance_cnt;
  scanf("%d", &instance_cnt);
  for (int i=0; i<instance_cnt; i++) {
    run_instance();
  }
  return 0;
}
