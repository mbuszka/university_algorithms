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
char           P[1000][1000][8];

void read_productions(int nt_cnt, int t_cnt) {
  nonterminal_productions = malloc(nt_cnt * sizeof(nproduction_t));
  terminal_produtions = malloc(t_cnt * sizeof(tproduction_t));

  for (int i=0; i<nt_cnt; i++) {
    char t, l, r;
    scanf(" %c %c %c", &t, &l, &r);
    nonterminal_productions[i].nterminal = t - 'A';
    nonterminal_productions[i].l = l - 'A';
    nonterminal_productions[i].r = r - 'A';
  }

  for (int i=0; i<t_cnt; i++) {
    char n, t;
    scanf(" %c %c ", &n, &t);
    terminal_produtions[i].nterminal = n - 'A';
    terminal_produtions[i].terminal  = t;
  }
}

void dump_productions(int nt_cnt, int t_cnt) {
  puts("Non-terminal productions\n");
  for (int i=0; i<nt_cnt; i++) {
    printf("%c -> %c%c\n", nonterminal_productions[i].nterminal + 'A'
                         , nonterminal_productions[i].l + 'A'
                         , nonterminal_productions[i].r + 'A' );
  }
  puts("\nTerminal productions\n");
  for (int i=0; i<t_cnt; i++) {
    printf("%c -> %c\n", terminal_produtions[i].nterminal + 'A'
                       , terminal_produtions[i].terminal );
  }
}

void dump_P(int word_len) {
  for (int l=0; l<word_len; l++) {
    for (int s=0; s<word_len; s++) {
      printf("{");
      for (int p=0; p<8; p++) {
        if (P[l][s][p]) printf("%c", 'A' + p);
      }
      printf("},\t");
    }
    printf("\n");
  }
}

int cyk(int word_len, int nt_cnt, int t_cnt) {
  bzero(P, 1000*1000*8);

  for (int s=0; s < word_len; s++) {
    for (int i=0; i<t_cnt; i++) {
      tproduction_t p = terminal_produtions[i];
      if (p.terminal == word[s])
        P[0][s][p.nterminal] = 1;
    }
  }

  for (int l=2; l<=word_len; l++)
    for (int s=0; s<=word_len-l; s++)
      for (int p=1; p<l; p++)
        for (int i=0; i<nt_cnt; i++) {
          nproduction_t prod = nonterminal_productions[i];
          if (P[p-1][s][prod.l] && P[l-p-1][s+p][prod.r])
            P[l-1][s][prod.nterminal] = 1;
        }
  return P[word_len-1][0][0];
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
