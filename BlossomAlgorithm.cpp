#include <iostream>
#include <vector>
#include <queue>

using namespace std;


class BlossomAlgorithm {
    int n = 0; // 顶点数
    vector<vector<int>> graph; // 邻接表
    vector<int> match; // 匹配数组
    vector<int> pre; // 前驱节点
    vector<int> base; // 基节点
    vector<int> color; // 节点颜色: -1=未访问, 0=外部点, 1=内部点
    vector<int> vis; // LCA访问标记
    int timer; // 时间戳

public:
    explicit BlossomAlgorithm(int n) : n(n), graph(n), match(n, -1),
                                       pre(n, -1), base(n), color(n, -1),
                                       vis(n, 0), timer(0) {
      for (int i = 0; i < n; i++) base[i] = i;
    }

    void add_edge(int u, int v) {
      graph[u].push_back(v);
      graph[v].push_back(u);
    }


    // 计算最大匹配
    int solve() {
      int count = 0;
      for (int i = 0; i < n; i++) {
        if (match[i] == -1 && find_augmenting_path(i)) {
          count++;
        }
      }
      return count;
    }

    // 获取匹配结果
    [[nodiscard]] vector<int> get_matches() const {
      return match;
    }

private:
    // 查找基节点 (带路径压缩)
    int find_base(int x) {
      while (base[x] != x) {
        base[x] = base[base[x]]; // 路径压缩
        x = base[x];
      }
      return x;
    }

    // 计算最近公共祖先 (LCA)
    int find_lca(int u, int v) {
      timer++; // 使用时间戳避免重置数组
      u = find_base(u);
      v = find_base(v);

      while (true) {
        if (u != -1) {
          if (vis[u] == timer) return u;
          vis[u] = timer;
          u = (match[u] == -1) ? -1 : find_base(pre[match[u]]);
        }
        if (u == -1 && v == -1) break;
        swap(u, v); // 交替向上
      }
      return -1;
    }

    // 收缩开花 (奇环)
    void blossom_contract(int u, int v, int lca, queue<int> &q) {
      while (find_base(u) != lca) {
        // 设置前驱关系
        pre[u] = v;
        v = match[u];

        // 如果v是内部点，将其转为外部点
        if (color[v] == 1) {
          color[v] = 0;
          q.push(v);
        }

        // 合并到开花
        base[u] = base[v] = lca;
        u = pre[v];
      }
    }

    // 从起点root寻找增广路径
    bool find_augmenting_path(int root) {
      fill(pre.begin(), pre.end(), -1);
      fill(color.begin(), color.end(), -1);
      for (int i = 0; i < n; i++) base[i] = i;

      queue<int> q;
      q.push(root);
      color[root] = 0; // 根节点为外部点

      while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v: graph[u]) {
          // 跳过自环或已在同一开花中的点
          if (v == u || find_base(u) == find_base(v)) continue;

          if (color[v] == -1) { // 未访问节点
            color[v] = 1; // 标记为内部点
            pre[v] = u;

            if (match[v] == -1) { // 找到未匹配点
              augment_path(v);
              return true;
            }

            // v已匹配，将其匹配点加入队列
            int w = match[v];
            color[w] = 0; // 外部点
            pre[w] = v;
            q.push(w);

          } else if (color[v] == 0) { // 遇到外部点，检测开花
            int lca = find_lca(u, v);
            if (lca == -1) continue; // 安全检查

            // 收缩开花
            blossom_contract(u, v, lca, q);
            blossom_contract(v, u, lca, q);
          }
        }
      }
      return false;
    }

    // 沿增广路径更新匹配
    void augment_path(int end) {
      while (end != -1) {
        int prev = pre[end];
        int next = match[prev];
        match[prev] = end;
        match[end] = prev;
        end = next;
      }
    }
};

int main() {
  int n, m;
  cin >> n >> m;
  BlossomAlgorithm solver(n);
  for (int i = 0; i < m; i++) {
    int x, y;
    cin >> x >> y;
    solver.add_edge(x - 1, y - 1);

  }
  int matching = solver.solve();
  cout << matching << endl;

  // 输出匹配结果
  vector<int> matches = solver.get_matches();
  for (int i = 0; i < n; i++) {
    cout << matches[i] + 1 << " ";
  }
  return 0;
}