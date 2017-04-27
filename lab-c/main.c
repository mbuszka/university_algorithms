#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nterminal;
  char l;
  char r;
} nproduction_t;

typedef struct {
  char nterminal;
  char terminal;
} tproduction_t;

nproduction_t *nonterminal_productions;
tproduction_t *terminal_produtions;
char           word[1001];
char           P[1000][1000];
char           nprods[8][8];
char           tprods[26];
char           avail_prods[64];
int            prod_cnt = 0;
char           tab[0x10000];

void read_productions(int nt_cnt, int t_cnt) {
  // nonterminal_productions = malloc(nt_cnt * sizeof(nproduction_t));
  terminal_produtions = malloc(t_cnt * sizeof(tproduction_t));
  bzero(nprods, 64);
  bzero(tprods, 26);
  bzero(avail_prods, 64);
  for (int k=0; k<nt_cnt; k++) {
    char t, l, r;
    scanf(" %c %c %c", &t, &l, &r);
    int i = l - 'A';
    int j = r - 'A';
    if (!nprods[i][j]) avail_prods[prod_cnt++] = (i << 3) | j;
    nprods[i][j] |= 1 << (t - 'A');
    // nonterminal_productions[i].nterminal = 1 << (t - 'A');
    // nonterminal_productions[i].l = 1 << (l - 'A');
    // nonterminal_productions[i].r = 1 << (r - 'A');
  }

  for (int k=0; k<prod_cnt; k++) {
    int i = avail_prods[k] >> 3;
    int j = avail_prods[k] & 7;
    if ((P[p-1][s] & (1 << i)) && (P[l-p-1][s+p] & (1 << j)))
      P[l-1][s] |= nprods[i][j];
  }

  for (int i=0; i<t_cnt; i++) {
    char n, t;
    scanf(" %c %c ", &n, &t);
    terminal_produtions[i].nterminal = 1 << (n - 'A');
    terminal_produtions[i].terminal  = t;
  }
}

void dump_productions(int nt_cnt, int t_cnt) {
  puts("Non-terminal productions\n");
  for (int i=0; i<nt_cnt; i++) {
    printf("%x -> %x%x\n", nonterminal_productions[i].nterminal
                         , nonterminal_productions[i].l
                         , nonterminal_productions[i].r );
  }
  puts("\nTerminal productions\n");
  for (int i=0; i<t_cnt; i++) {
    printf("%x -> %c\n", terminal_produtions[i].nterminal
                       , terminal_produtions[i].terminal );
  }
}

void dump_P(int word_len) {
  for (int l=0; l<word_len; l++) {
    for (int s=0; s<word_len; s++) {
      printf("{ %x }, ", P[l][s]);
    }
    printf("\n");
  }
}

int cyk(int word_len, int nt_cnt, int t_cnt) {
  bzero(P, 1000*1000);

  for (int s=0; s < word_len; s++) {
    for (int i=0; i<t_cnt; i++) {
      tproduction_t p = terminal_produtions[i];
      if (p.terminal == word[s])
        P[0][s] |= p.nterminal;
    }
  }

  for (int l=2; l<=word_len; l++)
    for (int s=0; s<=word_len-l; s++)
      for (int p=1; p<l; p++)
        for (int k=0; k<prod_cnt; k++) {
          int i = avail_prods[k] >> 3;
          int j = avail_prods[k] & 7;
          if ((P[p-1][s] & (1 << i)) && (P[l-p-1][s+p] & (1 << j)))
            P[l-1][s] |= nprods[i][j];
        }
  return P[word_len-1][0] & 1;
}



void run_instance() {
  int terminal_prod_cnt, nonterminal_prod_cnt, res, word_len;

  scanf("%d %d", &nonterminal_prod_cnt, &terminal_prod_cnt);
  read_productions(nonterminal_prod_cnt, terminal_prod_cnt);

  scanf("%1000s", word);
  // printf("%s\n", word);
  word_len = strlen(word);

  res = cyk(word_len, nonterminal_prod_cnt, terminal_prod_cnt);
  // dump_P(word_len);
  printf("%s\n", res ? "TAK" : "NIE");
  // dump_productions(nonterminal_prod_cnt, terminal_prod_cnt);

  free(nonterminal_productions);
  free(terminal_produtions);
}

int main() {
  int instance_cnt;
  scanf("%d", &instance_cnt);
  for (int i=0; i<instance_cnt; i++) {
    run_instance();
  }
  return 0;
}
