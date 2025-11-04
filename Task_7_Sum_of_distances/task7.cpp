#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {
int popcount64(uint64_t value) {
    int count = 0;
    while (value) {
        value &= value - 1;
        ++count;
    }
    return count;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) {
        return 0;
    }

    vector<string> matrix(n);
    for (int i = 0; i < n; ++i) {
        cin >> matrix[i];
    }

    const int words = (n + 63) / 64;
    vector<vector<uint64_t>> adj(n, vector<uint64_t>(words, 0));
    const uint64_t last_mask = (n % 64 == 0) ? ~0ULL : ((1ULL << (n % 64)) - 1);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == '1') {
                adj[i][j >> 6] |= 1ULL << (j & 63);
            }
        }
    }

    const long long n_sq = 1LL * n * n;
    vector<uint64_t> frontier(words, 0);
    vector<uint64_t> visited(words, 0);
    vector<uint64_t> reach(words, 0);

    auto clear_bitset = [&](vector<uint64_t> &bs) {
        fill(bs.begin(), bs.end(), 0ULL);
    };

    auto bitset_empty = [&](const vector<uint64_t> &bs) -> bool {
        for (uint64_t word : bs) {
            if (word != 0) {
                return false;
            }
        }
        return true;
    };

    long long total_sum = 0;

    for (int s = 0; s < n; ++s) {
        clear_bitset(frontier);
        clear_bitset(visited);
        clear_bitset(reach);

        frontier[s >> 6] |= 1ULL << (s & 63);
        visited[s >> 6] |= 1ULL << (s & 63);

        int visited_count = 1;
        long long source_sum = 0;
        int depth = 0;

        while (!bitset_empty(frontier)) {
            clear_bitset(reach);

            for (int w = 0; w < words; ++w) {
                uint64_t bits = frontier[w];
                if (!bits) {
                    continue;
                }
                int base_vertex = w << 6;
                for (int bit = 0; bit < 64 && base_vertex + bit < n; ++bit) {
                    if ((bits >> bit) & 1ULL) {
                        int vertex = base_vertex + bit;
                        for (int k = 0; k < words; ++k) {
                            reach[k] |= adj[vertex][k];
                        }
                    }
                }
            }

            bool has_new = false;
            for (int k = 0; k < words; ++k) {
                reach[k] &= ~visited[k];
                if (k == words - 1) {
                    reach[k] &= last_mask;
                }
                if (reach[k] != 0) {
                    has_new = true;
                }
            }

            if (!has_new) {
                break;
            }

            ++depth;
            long long level_count = 0;
            for (int k = 0; k < words; ++k) {
                uint64_t bits = reach[k];
                frontier[k] = bits;
                visited[k] |= bits;
                level_count += popcount64(bits);
            }

            visited_count += static_cast<int>(level_count);
            source_sum += level_count * depth * depth;
        }

        source_sum += 1LL * (n - visited_count) * n_sq;
        total_sum += source_sum;
    }

    cout << total_sum << '\n';
    return 0;
}
