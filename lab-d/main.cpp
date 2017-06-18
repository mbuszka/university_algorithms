#define DEBUG 0

#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>

using namespace std;

namespace std {
template <> struct hash<vector<size_t>> {
  std::size_t operator()(const vector<size_t>& v) const {
    size_t sumx = 0;
    size_t sum2 = 0;
    size_t sum3 = 0;
    for (size_t i=0; i < v.size(); i++) {
      size_t x = v[i];
      sumx ^= x;
      x %= 500009l;
      x *= x;
      x %= 500009l;
      sum2 += x;
      sum2 %= 500009l;
      x *= v[i] % 500009l;
      x %= 500009l;
      sum3 += x;
      sum3 %= 500009l;
    }
    return (sumx << 31) ^ (sum2 << 13) ^ sum3;
  }
};
}

void read_tree(int tree_size, vector<size_t> *tree) {
  if (DEBUG) printf("read_tree\n");
  for (int i=0; i<tree_size-1; i++) {
    size_t a, b;
    scanf("%lu %lu", &a, &b);
    a--; b--;
    tree[a].push_back(b);
    tree[b].push_back(a);
  }
}

int bfs(int *path, size_t tree_size, vector<size_t> *tree, size_t s) {
  queue<int> q;
  int        visited[tree_size];
  int        e = s;
  bzero(visited, sizeof(visited));
  visited[s] = 1;
  q.push(s);
  while (!q.empty()) {
    e = q.front();
    q.pop();
    for (size_t j=0; j<tree[e].size(); j++) {
      int c = tree[e][j];
      if (!visited[c]) {
        path[c] = e;
        visited[c] = 1;
        q.push(c);
      }
    }
  }
  return e;
}

int find_centroid(vector<size_t> *level, size_t tree_size, vector<size_t> *tree) {
  int paren[tree_size];
  int v = bfs(paren, tree_size, tree, 0);
  int w = bfs(paren, tree_size, tree, v);

  vector<size_t> path;
  while (w != v) {
    path.push_back(w);
    w = paren[w];
  }
  path.push_back(w);

  if (DEBUG) {
    printf("found path\n");
    for (size_t i=0; i<path.size(); i++) {
      printf("%lu ", path[i]);
    }
    printf("\n");
  }

  int n = tree_size;
  if (path.size() % 2 == 0) {
    // printf("tree_size %d, tree.size() %d\n", tree_size, tree[0].size());
    n++;
    size_t a = path[path.size() / 2];
    size_t b = path[path.size() / 2 - 1];

    for (size_t i=0; i<tree[a].size(); i++) {
      if (tree[a][i] == b) {
        tree[a][i] = tree_size;
        break;
      }
    }

    for (size_t i=0; i<tree[b].size(); i++) {
      if (tree[b][i] == a) {
        tree[b][i] = tree_size;
        break;
      }
    }

    tree[tree_size].push_back(a);
    tree[tree_size].push_back(b);
    level[0].push_back(tree_size);
  } else {
    level[0].push_back(path[path.size() / 2]);
  }
  return n;
}

int fill_levels(vector<size_t> *level, size_t tree_size, vector<size_t> *tree) {
  int visited[tree_size];
  bzero(visited, sizeof(visited));
  size_t i=0;
  for (;!level[i].empty(); i++) {
    for (size_t j=0; j < level[i].size(); j++) {
      int node = level[i][j];
      visited[node] = 1;
      for (size_t k=0; k < tree[node].size(); k++) {
        if (!visited[tree[node][k]]) {
          visited[tree[node][k]] = 1;
          level[i+1].push_back(tree[node][k]);
        }
      }
    }
  }
  return i;
}



void dump_levels(vector<size_t> *level) {
  for (size_t i=0; !level[i].empty(); i++) {
    for (size_t k=0; k < level[i].size(); k++) {
      printf("%d ", level[i][k]);
    }
    printf("\n");
  }
}

int next_code = 2;

void calculate_codes(size_t *codes,
                     size_t tree_size,
                     vector<size_t> *tree,
                     vector<size_t> *level,
                     int l) {
  int visited[tree_size];
  bzero(visited, sizeof(visited));
  // map.insert({{}, 1});

  for (int i=l-1; i >= 0; i--) {
    for (size_t j=0; j < level[i].size(); j++) {
      visited[level[i][j]] = 1;
      vector<size_t> children = tree[level[i][j]];
      if (children.size() == 1) {
        codes[level[i][j]] = 1;
      } else {
        vector<size_t> key;

        for (size_t i=0; i<children.size(); i++) {
          if (visited[children[i]]) key.push_back(codes[children[i]]);
        }
        if (DEBUG) printf("Node %d, hashing vec of size %d\n", level[i][j], key.size());
        codes[level[i][j]] = hash<vector<size_t>>{}(key);

        // unordered_map<vector<size_t>, int>::const_iterator got = map.find(key);
        //
        // if (got == map.end()) {
        //   map.insert({key, next_code});
        //   codes[level[i][j]] = next_code;
        //   next_code++;
        // } else {
        //   codes[level[i][j]] = got->second;
        // }
      }
    }
  }
}

void round() {
  if (DEBUG) printf("round\n");
  size_t tree_size;
  scanf("%lu", &tree_size);

  vector<size_t>         tree[2][tree_size+1];
  vector<size_t>         key[2][tree_size+1];
  size_t                 codes[2][tree_size+1];
  vector<size_t>         level[2][tree_size+1];

  if (DEBUG) printf("round\n");

  read_tree(tree_size, tree[0]);
  read_tree(tree_size, tree[1]);

  if (DEBUG) printf("read trees\n");

  int size[2];
  for (int i=0; i<2; i++) {
    size[i] = find_centroid(level[i], tree_size, tree[i]);
  }
  if (DEBUG) printf("found centroids\n");


  if (size[0] != size[1]) {
    printf("NIE\n");
    return;
  }
  tree_size = size[0];


  int level_cnt[2];
  for (int i=0; i<2; i++) {
    level_cnt[i] = fill_levels(level[i], tree_size, tree[i]);
    if (DEBUG) dump_levels(level[i]);
    if (DEBUG) printf("\n");
  }
  for (int i=0; i<2; i++) {
    calculate_codes(codes[i], tree_size, tree[i], level[i], level_cnt[i]);

    if (DEBUG) {
      for (size_t j=0; j<tree_size; j++) {
        printf("%lx ", codes[i][j]);
      }
      printf("\n");
    }
  }

  if (codes[0][level[0][0][0]] == codes[1][level[1][0][0]]) {
    printf("TAK\n");
  } else {
    printf("NIE\n");
  }
}

int main() {
  int test_cnt;
  scanf("%d", &test_cnt);
  for (int i=0; i < test_cnt; i++) {
    round();
  }
  return 0;
}
