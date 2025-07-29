#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

class GeneralWeightedMatching {
private:
    struct E { int u, v; long long w; };
    static constexpr long long INF = 1001001001001001001LL;

    int n, m, in;
    std::vector<std::vector<E>> G;
    std::vector<int> mate, slack, root, par, isS, used;
    std::vector<std::vector<int>> flower, belong;
    std::vector<long long> dual;
    std::queue<int> que;

    long long dist(const E& e) { return dual[e.u] + dual[e.v] - e.w; }

    void update(int u, int v) {
      if (!slack[v] || dist(G[u][v]) < dist(G[slack[v]][v])) {
        slack[v] = u;
      }
    }

    void recalc(int v) {
      slack[v] = 0;
      for (int i = 1; i <= n; i++) {
        if (G[i][v].w && root[i] != v && isS[root[i]] == 1) {
          update(i, v);
        }
      }
    }

    void push(int v) {
      if (v <= n) que.push(v);
      else for (int nxt : flower[v]) push(nxt);
    }

    void set(int v, int rt) {
      root[v] = rt;
      if (v > n) for (int nxt : flower[v]) set(nxt, rt);
    }

    int findeven(int b, int v) {
      auto it = std::find(flower[b].begin(), flower[b].end(), v);
      int pos = it - flower[b].begin();
      if (pos & 1) {
        std::reverse(flower[b].begin() + 1, flower[b].end());
        pos = flower[b].size() - pos;
      }
      return pos;
    }

    void match(int u, int v) {
      mate[u] = G[u][v].v;
      if (u > n) {
        int x = belong[u][G[u][v].u];
        int pos = findeven(u, x);
        for (int i = 0; i < pos; i++) {
          match(flower[u][i], flower[u][i^1]);
        }
        match(x, v);
        std::rotate(flower[u].begin(), flower[u].begin() + pos, flower[u].end());
      }
    }

    void link(int u, int v) {
      while (true) {
        int nv = root[mate[u]];
        match(u, v);
        if (!nv) break;
        v = nv;
        u = root[par[nv]];
        match(v, u);
      }
    }

    void make(int u, int v, int lca) {
      int id = n + 1;
      while (id <= m && root[id]) id++;
      if (id > m) m++;
      flower[id].clear();
      for (int i = 1; i <= m; i++) {
        G[id][i].w = 0;
        G[i][id].w = 0;
      }
      for (int i = 1; i <= n; i++) {
        belong[id][i] = 0;
      }
      isS[id] = 1;
      dual[id] = 0;
      mate[id] = mate[lca];
      while (u != lca) {
        flower[id].push_back(u);
        u = root[mate[u]];
        push(u);
        flower[id].push_back(u);
        u = root[par[u]];
      }
      flower[id].push_back(lca);
      std::reverse(flower[id].begin(), flower[id].end());
      while (v != lca) {
        flower[id].push_back(v);
        v = root[mate[v]];
        push(v);
        flower[id].push_back(v);
        v = root[par[v]];
      }
      set(id, id);
      for (int c : flower[id]) {
        for (int i = 1; i <= m; i++) {
          if (G[id][i].w == 0 || dist(G[c][i]) < dist(G[id][i])) {
            G[id][i] = G[c][i];
            G[i][id] = G[i][c];
          }
        }
        for (int i = 1; i <= n; i++) {
          if (belong[c][i]) belong[id][i] = c;
        }
      }
      recalc(id);
    }

    void expand(int b) {
      for (int c : flower[b]) set(c, c);
      int x = belong[b][G[b][par[b]].u];
      isS[x] = 2;
      par[x] = par[b];
      int pos = findeven(b, x);
      for (int i = 0; i < pos; i += 2) {
        int T = flower[b][i];
        int S = flower[b][i+1];
        isS[S] = 1;
        isS[T] = 2;
        par[T] = G[S][T].u;
        slack[S] = slack[T] = 0;
        push(S);
      }
      for (int i = pos + 1; i < flower[b].size(); i++) {
        isS[flower[b][i]] = 0;
        recalc(flower[b][i]);
      }
      flower[b].clear();
      root[b] = 0;
    }

    bool path(const E& e) {
      int u = root[e.u];
      int v = root[e.v];
      if (isS[v] == 0) {
        par[v] = e.u;
        isS[v] = 2;
        int nu = root[mate[v]];
        slack[v] = slack[nu] = 0;
        isS[nu] = 1;
        push(nu);
      } else if (isS[v] == 1) {
        int lca = 0;
        int bu = u, bv = v;
        in++;
        while (bu) {
          used[bu] = in;
          bu = root[mate[bu]];
          if (bu) bu = root[par[bu]];
        }
        while (bv) {
          if (used[bv] == in) {
            lca = bv;
            break;
          }
          bv = root[mate[bv]];
          if (bv) bv = root[par[bv]];
        }
        if (lca) {
          make(u, v, lca);
        } else {
          link(u, v);
          link(v, u);
          return true;
        }
      }
      return false;
    }

    bool augment() {
      isS.assign(2*n, 0);
      slack.assign(2*n, 0);
      par.assign(2*n, 0);
      que = std::queue<int>();
      for (int i = 1; i <= m; i++) {
        if (root[i] == i && mate[i] == 0) {
          isS[i] = 1;
          push(i);
        }
      }
      if (que.empty()) return false;
      while (true) {
        while (!que.empty()) {
          int v = que.front();
          que.pop();
          for (int i = 1; i <= n; i++) {
            if (G[v][i].w && root[i] != root[v]) {
              if (dist(G[v][i]) == 0) {
                if (path(G[v][i])) return true;
              } else if (isS[root[i]] != 2) {
                update(v, root[i]);
              }
            }
          }
        }
        long long delta = INF;
        for (int i = n+1; i <= m; i++) {
          if (root[i] == i && isS[i] == 2) {
            if (dual[i] / 2 < delta) delta = dual[i] / 2;
          }
        }
        for (int i = 1; i <= m; i++) {
          if (root[i] == i && slack[i] && isS[i] != 2) {
            long long d = dist(G[slack[i]][i]);
            if (isS[i] == 0) {
              if (d < delta) delta = d;
            } else {
              if (d / 2 < delta) delta = d / 2;
            }
          }
        }
        for (int i = 1; i <= n; i++) {
          if (isS[root[i]] == 1) {
            dual[i] -= delta;
            if (dual[i] <= 0) return false;
          } else if (isS[root[i]] == 2) {
            dual[i] += delta;
          }
        }
        for (int i = n+1; i <= m; i++) {
          if (root[i] == i && isS[i]) {
            if (isS[i] == 1) dual[i] += 2 * delta;
            else dual[i] -= 2 * delta;
          }
        }
        for (int i = 1; i <= m; i++) {
          if (root[i] == i && slack[i] && root[slack[i]] != i) {
            if (dist(G[slack[i]][i]) == 0) {
              if (path(G[slack[i]][i])) return true;
            }
          }
        }
        for (int i = n+1; i <= m; i++) {
          if (root[i] == i && isS[i] == 2 && dual[i] == 0) {
            expand(i);
          }
        }
      }
    }

public:
    GeneralWeightedMatching(int _n) : n(_n), m(n), in(0) {
      G = std::vector<std::vector<E>>(2*n, std::vector<E>(2*n));
      mate = std::vector<int>(2*n, 0);
      root = std::vector<int>(2*n, 0);
      used = std::vector<int>(2*n, 0);
      flower = std::vector<std::vector<int>>(2*n);
      belong = std::vector<std::vector<int>>(2*n, std::vector<int>(2*n, 0));
      dual = std::vector<long long>(2*n, 0);
      for (int i = 0; i <= n; i++) {
        root[i] = i;
        belong[i][i] = i;
        if (i) dual[i] = INF;
        for (int j = 0; j <= n; j++) {
          G[i][j] = E{i, j, 0};
        }
      }
    }

    void add_edge(int u, int v, long long w) {
      u++; v++;
      if (w*2 > G[u][v].w) G[u][v].w = w*2;
      if (w*2 > G[v][u].w) G[v][u].w = w*2;
    }

    std::vector<int> run() {
      while (augment());
      std::vector<int> res(n, -1);
      for (int i = 1; i <= n; i++) {
        if (mate[i]) res[i-1] = mate[i] - 1;
      }
      return res;
    }

    long long get_total_weight() {
      long long total = 0;
      for (int i = 1; i <= n; i++) {
        int j = mate[i];
        if (j != 0 && i < j && j <= n) {
          total += G[i][j].w / 2;
        }
      }
      return total;
    }
};

int main() {
  int n, m;
  cin >> n >> m;

  GeneralWeightedMatching solver(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;  // 输入权重使用 long long
    cin >> u >> v >> w;
    solver.add_edge(u-1, v-1, w);
  }

  auto match = solver.run();

  cout << solver.get_total_weight() << endl;
  for (auto x : match) {
    cout << x + 1 << " ";
  }
  return 0;
}