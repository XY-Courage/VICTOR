class PrimeEulerSieve {
    int n;
public:
    vector<int> min_prime;
    vector<bool> is_prime;
    vector<int> primes;

    explicit PrimeEulerSieve(int _n) : n(_n), min_prime(n + 1, 0), is_prime(n + 1, true) {
      is_prime[0] = is_prime[1] = false;
      for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
          primes.push_back(i);
          min_prime[i] = i;
        }

        for (int j = 0; j < primes.size(); ++j) {
          int p = primes[j];
          long long next = 1LL * i * p;
          if (next > n) break;
          is_prime[next] = false;
          min_prime[next] = p;
          if (i % p == 0) break;
        }
      }
    }

    vector<pair<int, int>> get_prime_factor(int x) {
      vector<pair<int, int>> result;
      while (x != 1) {
        int min_fac = min_prime[x];
        if (result.empty() || min_fac != result[result.size() - 1].first) {
          result.emplace_back(min_fac, 1);
        } else {
          result[result.size() - 1].second++;
        }
        x /= min_fac;
      }
      return result;
    }
};
const int PRIME_N = 1e6;
PrimeEulerSieve prime_helper(PRIME_N);