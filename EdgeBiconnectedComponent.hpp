/*
 * 边双连通分量 O(n+m)
 * 0 - base
 * 使用atcoder::scc实现强联通分量部分
 * add_edge 允许重边和自环
 * 不要求全图连通
 */
class EdgeBiconnectedComponent {
private:
    int n = 0;
    vector<vector<pair<int, int>>> G;
    vector<int> edge_used;
    atcoder::scc_graph scc;
public:
    explicit EdgeBiconnectedComponent(int _n) : n(_n), scc(_n), G(_n) {
    }

    void add_edge(int u, int v) {
      if (u == v) return;
      G[u].emplace_back(v, edge_used.size());
      G[v].emplace_back(u, edge_used.size());
      edge_used.emplace_back(false);
    }

    vector<vector<int>> ebc() {
      vector<int> vis(n);
      function<void(int, int)> dfs = [&](int u, int fa) -> void {
          vis[u] = true;
          bool vis_fa = false;
          for (auto [v, edge_ind]: G[u]) {
            if (edge_used[edge_ind]) continue;
            edge_used[edge_ind] = true;
            scc.add_edge(u, v);
            if (vis[v]) continue;
            dfs(v, u);
          }
      };
      for (int i = 0;i < n;i++) {
        if (vis[i]) continue;
        dfs(i, -1);
      }
      auto result = scc.scc();
      return result;
    }
    vector<vector<pair<int, int>>> get_G() {
      return G;
    }
};