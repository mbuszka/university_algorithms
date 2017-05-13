#include <stdio.h>
#include <vector>
#include <queue>

#define N 500002

using namespace std;

vector<int>         tree[2][N];
vector<int>         key[2][N];
int                 code[2][N];
vector<int>         level[2][N];
int                 tree_size;

void read_trees() {
  scanf("%d", &tree_size);
  for (int j=0; j<2; j++) {
    for (int i=0; i<tree_size; i++) {
      int a, b;
      scanf("%d %d", &a, &b);
      tree[j][a].push_back(b);
      tree[j][b].push_back(a);
    }
  }
}

int bfs(int *path, int t, int s) {
  queue<int> q;
  int        e = s;
  q.push(s);
  while (!q.empty()) {
    e = q.front();
    q.pop();
    for (size_t j=0; j<tree[t][e].size(); j++) {
      int c = tree[t][e][j];
      path[c] = e;
      q.push(c);
    }
  }
  return e;
}

int find_centroid(int t) {
  int paren[tree_size + 1];
  int v = bfs(paren, t, 1);
  int w = bfs(paren, t, v);
  vector<int> path;
  while (paren[w] != v) {
    path.push_back(w);
    w = paren[w];
  }
  int n = tree_size;
  if (path.size() % 2 == 0) {
    n++;
    tree[t][n].push_back(path[path.size() / 2]);
    tree[t][n].push_back(path[path.size() / 2 - 1]);
    level[t][0].push_back(tree_size);
  } else {
    level[t][0].push_back(path[path.size() / 2]);
  }
  return path.size();
}

int main() {
  read_trees();
  int len0, len1;
  len0 = find_centroid(0);
  len1 = find_centroid(1);
  if (len0 != len1) {
    printf("NIE\n");
    return;
  }


  printf("hello world\n");
}
