#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 2000001
#define DEBUG 0
#define TRACE 0

typedef struct {
  int v;
  int e;
  int s;
} cell_t;

typedef struct {
  int v;
  int idx;
} interm_t;

cell_t   tab[MAX_LEN];
interm_t input[MAX_LEN];

int tree_size;
int tree[0x800000];

void print_cell(cell_t *c) {
  printf("v: %d, e: %d, s: %d\n", c->v, c->e, c->s);
}

int compare_interm(const void *a, const void *b) {
  interm_t *left = (interm_t*) a;
  interm_t *right = (interm_t*) b;
  return left->v - right->v;
}

void load_table(interm_t *table, int n) {
  for (int i = 0; i < n; i ++) {
    scanf("%d", &table[i].v);
    table[i].idx = i;
  }
}

int renumber(interm_t *src, cell_t *dst, int n) {
  qsort(src, n, sizeof(interm_t), compare_interm);
  int v = 1;
  interm_t *now = src, *prev = src;
  while (now != src + n) {
    if (now->v != prev->v) v ++;
    dst[now->idx].v = v;
    prev = now;
    now ++;
  }
  return v;
}

void preprocess(cell_t *table, int n) {
  int beg = 0, end = 0;
  while (beg < n) {
    if (beg == end) {
      table[beg].e = 1;
      end ++;
    }
    while (end < n && table[end-1].v < table[end].v) {
      table[end].e = end - beg + 1;
      end ++;
    }
    table[beg].s = end - beg;
    beg ++;
  }
}

void find_tree_size(int n) {
  tree_size = 1;
  while (tree_size < n) tree_size *= 2;
  tree_size *= 2;
}

void insert(int x, int v) {
  int low = 1, high = tree_size / 2;
  int idx = 1;
  do {
    tree[idx] = tree[idx] < v ? v : tree[idx];
    int mid = (low + high) / 2;
    if (x > mid) {
      low = mid + 1;
      idx = idx * 2 + 1;
    } else {
      high = mid;
      idx *= 2;
    }
  } while (low != high);
  tree[idx] = tree[idx] < v ? v : tree[idx];
}

int query(int x) {
  if (x < 1) return 0;
  int low = 1, high = tree_size / 2;
  int idx = 1;
  int max = 0;
  do {
    int mid = (low + high) / 2;
    if (mid < x) {
      low = mid + 1;
      idx = idx * 2 + 1;
      max = max < tree[idx - 1] ? tree[idx - 1] : max;
    } else {
      high = mid;
      idx *= 2;
    }
  } while (high != x);
  max = max < tree[idx] ? tree[idx] : max;
  return max;
}

void print_tree() {
  int i = 1;
  int k = 1;
  while (i < tree_size) {
    for (int j = 0; j < k; j ++) printf("%d ", tree[i + j]);
    i += k;
    k *= 2;
    printf("\n");
  }
}

void run_case() {
  int len;
  scanf("%d", &len);
  memset(input, 0, len * sizeof(interm_t));
  memset(tab, 0, len * sizeof(cell_t));
  load_table(input, len);
  int vals = renumber(input, tab, len);
  
  preprocess(tab, len);
  find_tree_size(vals);
  if (DEBUG) printf("vals: %d, tree_size: %d\n", vals, tree_size); 
  memset(tree, 0, tree_size * sizeof(int));

  int max = 0;
  for (int i = 0; i < len; i ++) {
    int q = query(tab[i].v - 1);
    int dist = q + tab[i].s;
    if (DEBUG) {
      printf("Checking cell %d\n", i);
      print_cell(&tab[i]);
      printf("max: %d, q: %d\n", max, q);
      printf("\n");
      print_tree();
      printf("\n\n");
    }
    insert(tab[i].v, tab[i].e);
    max = dist > max ? dist : max;
  }
  printf("%d\n", max);
  
}

int main() {
  int test_cases = 1;
  scanf("%d", &test_cases);
  for (int i = 0; i < test_cases; i ++) {
    run_case();
  }
}

