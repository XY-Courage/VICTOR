// UOJ 117 Passed Code
#include"bits/stdc++.h"

using namespace std;

// 特别地，使用edge_result时，边的编号是 1-base
class EulerianPath {
private:
    int n = 0;
    vector<vector<pair<int, int>>> G;
    vector<int> in_degree;

    // edge_size 为 1-base 用于区分正反边
    int edge_size = 0;
    bool is_directed;
    vector<int> edge_result;
public:
    explicit EulerianPath(int _n, bool _is_directed = true) : n(_n), G(n), in_degree(n, 0),
                                                              is_directed(_is_directed) {}

    void add_edge(const int u, const int v) {
      edge_size++;
      G[u].emplace_back(v, edge_size);
      in_degree[v]++;
      // 无向图建双向边
      if (!is_directed) {
        G[v].emplace_back(u, -edge_size);
        in_degree[u]++;
      }
    }

    vector<int> solve(int minimal_dictionary = false) {
      vector<int> result;
      edge_result.clear();
      // 要求字典序最小
      if (minimal_dictionary) {
        for (int i = 0; i < n; i++) {
          ranges::sort(G[i]);
        }
      }

      // 确定欧拉路是否存在，并同时确定起点
      int begin = -1;
      int end = -1;
      // 单向/无向图分别实现
      if (is_directed) {
        for (int i = 0; i < n; i++) {
          // 该点为起点
          if (G[i].size() - in_degree[i] == 1) {
            // 多个起点
            if (begin != -1) return {};
            begin = i;
          } else if (in_degree[i] - G[i].size() == 1) {
            // 该点为终点
            if (end != -1) return {};
            end = i;
          } else if (in_degree[i] != G[i].size()) {
            // 异常点
            return {};
          }
        }
      } else {
        for (int i = 0; i < n; i++) {
          // 是否为奇数点
          if (G[i].size() % 2 == 1) {
            if (begin != -1) {
              if (end != -1) {
                // 多个奇数点
                return {};
              }
              end = i;
            }
            begin = i;
          }
        }
      }

      // 存在欧拉回路，以第一个有出边的点为起点
      if (begin == -1) {
        for (int i = 0; i < n; i++) {
          if (G[i].size() != 0) {
            begin = i;
            break;
          }
        }
        // 图中无边，返回{0}
        if (begin == -1) {
          return {0};
        }
      }

      vector<int> edge_used(edge_size + 1);
      vector<int> current_edge(n);
      function<void(int, int)> hierholzer = [&](int u, int from_edge_ind) {
          for (int &i = current_edge[u]; i < G[u].size();) {
            auto [v, edge_ind] = G[u][i];
            if (!edge_used[abs(edge_ind)]) {
              edge_used[abs(edge_ind)] = true;
              i++;
              hierholzer(v, edge_ind);
            } else {
              i++;
            }
          }
          result.emplace_back(u);
          edge_result.emplace_back(from_edge_ind);
      };
      hierholzer(begin, 0);
      edge_result.pop_back();

      // 欧拉路长度错误
      if (result.size() != edge_size + 1) {
        return {};
      }
      ranges::reverse(result);
      ranges::reverse(edge_result);
      return result;
    };

    vector<int> get_edge_result() {
      return edge_result;
    }
};

int main() {
  int op;
  cin >> op;
  bool is_directed;
  if (op == 1) is_directed = false;
  else is_directed = true;

  int n, m;
  cin >> n >> m;
  EulerianPath solver(n, is_directed);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    solver.add_edge(u - 1, v - 1);
  }
  auto result = solver.solve(true);
  if (result.empty() || result[0] != result[result.size() - 1]) {
    cout << "NO";
  } else {
    cout << "YES\n";
    auto edge_result = solver.get_edge_result();
    for (auto e: edge_result) {
      cout << e << " ";
    }
  }
}