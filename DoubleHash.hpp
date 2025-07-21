class DoubleHash {
private:
    int n = 0;
    vector<mint> pre1, pre2;
    vector<mint> base1, base2;

    static vector<int> string_to_vector(const string &s) {
      vector<int> v;
      for (auto c: s) {
        v.emplace_back(c);
      }
      return v;
    }

public:
    DoubleHash(const vector<int> &v) {
      n = v.size();
      pre1.resize(n);
      pre2.resize(n);
      base1.resize(n);
      base2.resize(n);
      for (int i = 0; i < n; i++) {
        base1[i] = (i == 0 ? 1 : base1[i - 1] * 131);
        base2[i] = (i == 0 ? 1 : base2[i - 1] * 137);
        pre1[i] = v[i] * base1[i] + (i == 0 ? 0 : pre1[i - 1]);
        pre2[i] = v[i] * base2[i] + (i == 0 ? 0 : pre2[i - 1]);
      }
    }

    DoubleHash(const string &s) : DoubleHash(string_to_vector(s)) {}

    pair<int, int> sum(const int l, const int r) {
      return {((pre1[r] - (l == 0 ? 0 : pre1[l - 1])) / base1[l]).val(),
              ((pre2[r] - (l == 0 ? 0 : pre2[l - 1])) / base2[l]).val()};
    }
};