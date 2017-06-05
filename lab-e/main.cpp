#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <stdint.h>
#include <string.h>

#define TRACE 0
#define DEBUG 0
#define LEVEL_MARK 1001
#define TABLE_LEN 1500007

int table_len = TABLE_LEN;
int len_lookup[8] = { 101, 101, 101, 1500007, 1500007, 1500007, 1500007, 1500007 };

typedef struct {
  uint32_t next;
  uint16_t b[7];
} buckets_t;

uint16_t capacities[7];

int buckets_len = 7;
buckets_t storage[400000];
uint32_t next_free = 0;
uint32_t hash_tbl[TABLE_LEN];

#define EMPTY 4000001
#define MARKER 2000000

typedef uint32_t buckets_ptr;

void cons(buckets_ptr head, buckets_ptr tail) {
  storage[head].next = tail;
}

uint32_t hash(buckets_ptr bucket) {
  uint32_t s = 0;
  uint32_t x = 57;
  for (int i = 0; i < buckets_len; i++) {
    s += storage[bucket].b[i] * x;
    x *= 57;
  }
  return s % table_len;
}

std::queue<uint32_t> rest;

int level = 0;
int count = 1;

int equals(buckets_ptr a, buckets_ptr b) {
  for (int i = 0; i < buckets_len; i++) 
    if (storage[a].b[i] != storage[b].b[i]) return 0;
  return 1;
}

int insert_unique(buckets_ptr b) {
  uint32_t h = hash(b);
  buckets_ptr buckets;
  if (hash_tbl[h] == EMPTY) {
    hash_tbl[h] = b;
    return 1;
  }

  buckets = hash_tbl[h];
  while (buckets != EMPTY) {
    if (equals(b, buckets)) return 0;
    buckets = storage[buckets].next;
  }

  cons(b, hash_tbl[h]);
  hash_tbl[h] = b;
  return 1;
}

void print_buckets(buckets_ptr b) {
  if (b == MARKER) {
    printf("MARKER\n");
    return;
  }
  printf("%u: [ ", b);
  for (int i = 0; i < buckets_len; i++) {
    printf("%hu ", storage[b].b[i]);
  }
  printf("] -> ");
  if (storage[b].next == EMPTY) {
    printf("[]\n");
  } else {
    printf("%u\n", storage[b].next);
  }
}

void add_drains(buckets_ptr b) {
  for (int i = 0; i < buckets_len; i++) {
    memcpy(&storage[next_free], &storage[b], sizeof(buckets_t));
    storage[next_free].next = EMPTY;
    storage[next_free].b[i] = 0;
    if (insert_unique(next_free)) {
      count ++;
      rest.push(next_free);
      next_free ++;
    }
  }
}

void add_flows(buckets_ptr b) {
  for (int i = 0; i < buckets_len; i++) {
    for (int j = 0; j < buckets_len; j ++) {
      if (i == j) continue;
      memcpy(&storage[next_free], &storage[b], sizeof(buckets_t));
      storage[next_free].next = EMPTY;
      uint32_t total = storage[b].b[i] + storage[b].b[j];
      storage[next_free].b[j] = total <= capacities[j] ? total : capacities[j];
      storage[next_free].b[i] = total <= capacities[j] ? 0 : total - capacities[j];
      if (insert_unique(next_free)) {
        count ++;
        rest.push(next_free);
        next_free ++;
      }
    }
  }
}


void bfs() {
  while (!rest.empty()) {
    buckets_ptr b;
    b = rest.front();
    if (TRACE) print_buckets(b);
    rest.pop();
    if (b == MARKER) {
      level ++;
      if (rest.back() != MARKER)
        rest.push(MARKER);
      continue;
    }
    add_drains(b);
    add_flows(b);
  }
  level --;
}

void hash_tbl_stats() {
  int cnt = 0;
  int max = 0;
  for (int i = 0; i < table_len; i++) {
    if (hash_tbl[i] == EMPTY) continue;
    cnt ++;
    int sum = 0;
    buckets_ptr bptr = hash_tbl[i];
    while (bptr != EMPTY) {
      sum ++;
      bptr = storage[bptr].next;
    }
    max = max < sum ? sum : max;
  }
  double avg_filled = (double) cnt / (double) table_len;
  double avg_per_slot = (double) count / (double) cnt;

  printf("Hashtable usage statistics\n"
         "slots filled: %d with average of %lf\n"
         "average len per slot %lf, max %d", cnt, avg_filled, avg_per_slot, max);
}

void run_set() {
  next_free = 0;
  count = 1;
  level = 0;
  scanf("%d", &buckets_len);
  table_len = len_lookup[buckets_len];
  for (int i = 0; i < buckets_len; i++) {
    scanf("%hu", &capacities[i]);
    storage[0].b[i] = capacities[i];
  }
  if (buckets_len == 1) {
    printf("2 1\n");
    return;
  }
  storage[0].next = EMPTY;
  
  for (int i = 0; i < table_len; i ++) {
    hash_tbl[i] = EMPTY;
  }

  insert_unique(0);
  next_free ++;
  rest.push(0);
  rest.push(MARKER);

  bfs();
  if (DEBUG) printf("created total of %d sets\n", next_free);
  if (DEBUG) hash_tbl_stats();
  printf("%d %d\n", count, level);
}

int main() {
  int test_cnt;
  scanf("%d", &test_cnt);
  for (int i = 0; i < test_cnt; i++) {
    run_set();
  }
  return 0;
}


