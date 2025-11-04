#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

static unsigned long long count_leq(unsigned long long x, unsigned long long mod, unsigned long long residue) {
    if (x < residue) {
        return 0ULL;
    }
    return (x - residue) / mod + 1;
}

static unsigned long long count_in_range(unsigned long long left, unsigned long long right, unsigned long long mod, unsigned long long residue) {
    if (left > right) {
        return 0ULL;
    }
    unsigned long long up_to_right = count_leq(right, mod, residue);
    unsigned long long up_to_left = (left == 0) ? 0ULL : count_leq(left - 1, mod, residue);
    return up_to_right - up_to_left;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    if (!(cin >> k)) {
        return 0;
    }

    vector<int> bits(k);
    for (int i = 0; i < k; ++i) {
        cin >> bits[i];
    }

    unsigned long long n;
    cin >> n;

    unsigned long long start = 0;
    for (int i = 0; i < k; ++i) {
        start = (start << 1) | static_cast<unsigned long long>(bits[i] & 1);
    }

    unsigned long long left = start;
    unsigned long long right = start + n - 1;

    unsigned long long total_flips = 0;
    for (int r = 0; r < k; ++r) {
        unsigned long long mod = 1ULL << r;
        unsigned long long residue = mod - 1;
        total_flips += count_in_range(left, right, mod, residue);
    }

    int max_bits = 1;
    for (int r = k - 1; r >= 0; --r) {
        unsigned long long mod = 1ULL << (r + 1);
        unsigned long long residue = (1ULL << r) - 1;
        if (count_in_range(left, right, mod, residue) > 0) {
            max_bits = r + 1;
            break;
        }
    }

    unsigned long long g = std::gcd(total_flips, n);

    cout << max_bits << '\n';
    cout << (total_flips / g) << '/' << (n / g) << '\n';

    return 0;
}
