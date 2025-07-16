class Dijkstra {
private:
    int n = 0;
    vector<long long> dis;
    vector<int> vis;
    vector<vector<pair<int, long long>>> G;
public:
    explicit Dijkstra(int _n) : n(_n), vis(n), dis(n, -1), G(n) {}

    void add_edge(const int u, const int v, const long long w) {
      G[u].emplace_back(v, w);
    }

    vector<long long> solve(const int ori) {
      priority_queue<pair<long long,int>> pq;
      dis[ori] = 0;
      pq.emplace(0,ori);
      while(!pq.empty()) {
        auto [dis_u,u] = pq.top();
        pq.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        for (auto [v,w] : G[u]) {
          if (dis[v] != -1 && dis[v] <= dis[u] + w) continue;
          dis[v] = dis[u] + w;
          pq.emplace(-dis[v],v);
        }
      }
      return dis;
    }
};