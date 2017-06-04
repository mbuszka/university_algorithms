#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <stdint.h>
#include <string.h>

#define DEBUG 1
#define LEVEL_MARK 1001
#define TABLE_LEN 1500007

typedef struct {
  uint32_t next;
  uint16_t b[7];
} buckets_t;

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
  for (int i = 0; i < buckets_len; i++) {
    int offset = i & 1 ? 16 : 0;
    s ^= storage[bucket].b[i] << offset;
  }
  return s % TABLE_LEN;
}

std::queue<uint32_t> rest;

int level = 0;
int count = 0;

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

void bfs() {
  while (!rest.empty()) {
    buckets_ptr b;
    b = rest.front();
    if (DEBUG) printf("Buckets: %u\n", b);
    rest.pop();
    if (b == MARKER) {
      level ++;
      if (rest.back() != MARKER)
        rest.push(MARKER);
      continue;
    }

    for (int i = 0; i < buckets_len; i++) {
      memcpy(&storage[next_free], &storage[b], sizeof(buckets_t));
      storage[next_free].b[i] = 0;
      if (insert_unique(next_free)) {
        count ++;
        rest.push(next_free);
        next_free ++;
      }
    }
  }
  level --;
}

void run_set() {
  next_free = 0;
  scanf("%d", &buckets_len);
  
  for (int i = 0; i < buckets_len; i++) {
    scanf("%hu", &storage[0].b[i]);
  }
  
  for (int i = 0; i < TABLE_LEN; i ++) {
    hash_tbl[i] = EMPTY;
  }

  insert_unique(0);
  rest.push(0);
  rest.push(MARKER);

  bfs();
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


