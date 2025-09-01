// 洛谷 P3806 AC代码
#include "bits/stdc++.h"

using namespace std;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
const int INF = 1e9;

void solve() {

  // ---- 读树部分，m 非树参数
  int n, m;
  cin >> n >> m;
  using edge = int; // edge 为树上边的信息
  vector<map<int, edge>> G(n);
  for (int i = 0; i < n - 1; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    u--;
    v--;
    G[u][v] = w;
    G[v][u] = w;
  }

  vector<int> ques(m);
  for (int i = 0; i < m; i++) {
    cin >> ques[i];
  }
  vector<int> ans(m, 0);

  const int MAXK = 1e7;
  vector<int> dis(MAXK + 1);

  // -------------- Begin 点分治核心代码 ------------------
  vector<int> tree_weight(n);
  vector<int> max_subtree_weight(n);
  function<void(int)> CentroidDecomposition = [&](int ori) {
      // 找当前重心
      int root = ori;
      vector<int> node_ind;
      function<void(int, int)> find_centroid_dfs = [&](int u, int fa) {
          node_ind.emplace_back(u);
          tree_weight[u] = 1;
          max_subtree_weight[u] = 0;
          for (auto [v, e]: G[u]) {
            if (v == fa) continue;
            find_centroid_dfs(v, u);
            tree_weight[u] += tree_weight[v];
            max_subtree_weight[u] = max(max_subtree_weight[u], tree_weight[v]);
          }
      };
      find_centroid_dfs(ori,-1);
      int size = static_cast<int>(node_ind.size());
      int min_max_subtree_weight = INT_MAX;
      for (auto u: node_ind) {
        max_subtree_weight[u] = max(max_subtree_weight[u], size - tree_weight[u]);
        if (max_subtree_weight[u] >= min_max_subtree_weight) continue;
        min_max_subtree_weight = max_subtree_weight[u];
        root = u;
      }

      // 用 root 做需要的分治任务
      dis[0] = 1;

      vector<int> all_len;
      for (auto [son, son_e]: G[root]) {
        vector<int> pending_add;
        function<void(int, int, int)> work_dfs = [&](int u, int fa, int len) {
            if (len > MAXK) return ;
            for (int i = 0; i < m; i++) {
              if (len > ques[i]) continue;
              if (dis[ques[i] - len]) ans[i] = 1;
            }
            pending_add.emplace_back(len);
            all_len.emplace_back(len);

            for (auto [v, e]: G[u]) {
              if (v == fa) continue;
              work_dfs(v, u, len + e);
            }
        };
        work_dfs(son, root, son_e);
        for (auto len : pending_add) {
          dis[len] ++;
        }
      }
      for (auto len : all_len) {
        dis[len] = 0;
      }

      // 继续分治
      for (auto [v, e]: G[root]) {
        G[v].erase(root);
        CentroidDecomposition(v);
      }

  };
  CentroidDecomposition(0);
  // -------------- End 点分治核心代码 ------------------
  for (int i = 0;i < m;i++) {
    if (ans[i]) cout << "AYE" << endl;
    else cout << "NAY" << endl;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T = 1;
//  cin >> T;
  while (T--) {
    solve();
  }
  return 0;
}