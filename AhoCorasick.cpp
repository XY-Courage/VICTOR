// Luogu P5357 Passed Code
#include"bits/stdc++.h"

using namespace std;

/*
 * 采用固定字母表大小的实现，实现了确定有限自动机
 * 其中 son 数组为自动机转移方程
 * flag 数组通常用于记录匹配串的信息，可自行定义
 * cnt 是默认统计出现次数的计数变量
 * 通常情况下，build后提取出 nodes 数组即可使用到全部信息
 */


class AhoCorasick {
private:
    int alphabet_size = 0;

    class Node {
    public:
        int fail;
        vector<int> son;
        vector<int> flag;
        int cnt = 0;

        explicit Node(int _alphabet_size) : fail(-1), son(_alphabet_size, -1), flag(), cnt(0) {}
    };

public:
    int size = 1;
    vector<Node> nodes;

    explicit AhoCorasick(int _alphabet_size) : alphabet_size(_alphabet_size), size() {
      size = 1;
      nodes.emplace_back(_alphabet_size);
    }

    void add(const vector<int> &v, const int flag = 1) {
      int now_index = 0;
      for (auto alpha: v) {
        assert(0 <= alpha && alpha < alphabet_size);
        // 获取当前节点的son[alpha]的值
        int next_index = nodes[now_index].son[alpha];
        if (next_index == -1) {
          // 需要创建新节点
          next_index = size;
          nodes[now_index].son[alpha] = next_index; // 将当前节点的son[alpha]指向新节点
          nodes.emplace_back(alphabet_size);
          size++;
        }
        now_index = next_index;
      }
      // --- 字符串信息，默认实现存储 flag 标记 ----
      nodes[now_index].flag.emplace_back(flag);
    }

    void add(const string &s, const char base_alpha, const int flag = 1) {
      vector<int> v;
      for (auto c: s) {
        v.emplace_back(c - base_alpha);
      }
      add(v, flag);
    }

    void build() {
      queue<int> q;
      nodes[0].fail = 0;  // 设置根节点的fail指向自己
      for (int alpha = 0; alpha < alphabet_size; alpha++) {
        if (nodes[0].son[alpha] == -1) nodes[0].son[alpha] = 0;
        else {
          int son_index = nodes[0].son[alpha];
          q.push(son_index);
          nodes[son_index].fail = 0;
        }
      }
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int alpha = 0; alpha < alphabet_size; alpha++) {
          if (nodes[u].son[alpha] != -1) {
            nodes[nodes[u].son[alpha]].fail = nodes[nodes[u].fail].son[alpha];
            q.push(nodes[u].son[alpha]);
          } else
            nodes[u].son[alpha] = nodes[nodes[u].fail].son[alpha];
        }
      }
    }
};

int main() {
  // 输入与构建 AC 自动机
  int n;
  AhoCorasick ac(26);
  cin >> n;
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    ac.add(s, 'a', i);
  }
  ac.build();

  // query 实现
  string s;
  cin >> s;
  int now_index = 0;
  for (auto c: s) {
    now_index = ac.nodes[now_index].son[c - 'a'];
    ac.nodes[now_index].cnt++;
  }

  int size = static_cast<int>(ac.nodes.size());
  vector<int> ans(n);
  vector<int> degree(size);
  queue<int> q;
  auto nodes = ac.nodes;
  for (int i = 0; i < size; i++) {
    degree[nodes[i].fail]++;
  }
  for (int i = 0; i < size; i++) {
    if (degree[i] == 0) {
      q.push(i);
    }
  }
  while (!q.empty()) {
    int index = q.front();
    q.pop();
    for (auto ind: nodes[index].flag) {
      ans[ind] += nodes[index].cnt;
    }
    int fail_index = nodes[index].fail;
    nodes[fail_index].cnt += nodes[index].cnt;
    degree[fail_index]--;
    if (!degree[fail_index]) {
      q.push(fail_index);
    }
  }
  for (auto x: ans) {
    cout << x << endl;
  }
}