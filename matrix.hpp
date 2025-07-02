using mint = atcoder::modint998244353;

class matrix {
public:
    // 构造函数：分配 size*size 的空间
    matrix(int size) : size_(size), a(size_, std::vector<mint>(size_, 0)) {}

    // 创建单位矩阵的静态工厂方法
    static matrix identity(int size) {
      matrix result(size);
      for (int i = 0; i < size; i++)
        result.a[i][i] = 1; // 主对角线设为1
      return result;
    }

    // 重载 * 运算符用于矩阵乘法
    matrix operator*(const matrix &other) const {
      if (size_ != other.size_) {
        throw std::invalid_argument("Matrix sizes must be equal for multiplication");
      }
      matrix result(size_);
      for (int i = 0; i < size_; i++)
        for (int j = 0; j < size_; j++)
          for (int k = 0; k < size_; k++)
            result.a[i][j] += a[i][k] * other.a[k][j];
      return result;
    }

    // 矩阵快速幂函数
    matrix pow(long long exponent) const {
      if (exponent < 0) throw std::invalid_argument("Exponent must be non-negative");

      // 特殊情况处理
      if (exponent == 0) return identity(size_);
      if (exponent == 1) return *this;

      // 快速幂算法
      matrix result = identity(size_);
      matrix base = *this; // 当前幂的底数
      while (exponent > 0) {
        if (exponent % 2 == 1) {
          result = result * base; // 当指数为奇数时乘入结果
        }
        base = base * base; // 底数平方
        exponent /= 2;     // 指数减半
      }
      return result;
    }

    int result(const int n) {
      mint res = 0;
      for (int i = 0; i < size_; i++) res += a[i][n];
      return res.val();
    }

    int size_;
    vector<vector<mint>> a;
};