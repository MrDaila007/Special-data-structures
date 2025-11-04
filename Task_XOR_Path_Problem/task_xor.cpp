/*
 * Counts XOR-equal paths in a grid using meet-in-the-middle.
 */
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::uint64_t k = 0;
    if (!(std::cin >> n >> m >> k)) {
        return 0;
    }

    std::vector<std::vector<std::uint64_t>> a(n, std::vector<std::uint64_t>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> a[i][j];
        }
    }

    const int total_steps = n + m - 2;
    const int left_steps = total_steps / 2;
    const int right_steps = total_steps - left_steps;

    std::vector<std::vector<std::unordered_map<std::uint64_t, std::uint64_t>>> prefix(
        n, std::vector<std::unordered_map<std::uint64_t, std::uint64_t>>(m));

    const auto dfs_left = [&](auto&& self, int i, int j, int steps, std::uint64_t value) -> void {
        if (steps == left_steps) {
            ++prefix[i][j][value];
            return;
        }
        if (i + 1 < n) {
            self(self, i + 1, j, steps + 1, value ^ a[i + 1][j]);
        }
        if (j + 1 < m) {
            self(self, i, j + 1, steps + 1, value ^ a[i][j + 1]);
        }
    };

    dfs_left(dfs_left, 0, 0, 0, a[0][0]);

    std::uint64_t answer = 0;
    const auto dfs_right = [&](auto&& self, int i, int j, int steps, std::uint64_t suffix) -> void {
        if (steps == right_steps) {
            const auto needed = k ^ suffix;
            const auto it = prefix[i][j].find(needed);
            if (it != prefix[i][j].end()) {
                answer += it->second;
            }
            return;
        }
        const std::uint64_t next_suffix = suffix ^ a[i][j];
        if (i > 0) {
            self(self, i - 1, j, steps + 1, next_suffix);
        }
        if (j > 0) {
            self(self, i, j - 1, steps + 1, next_suffix);
        }
    };

    dfs_right(dfs_right, n - 1, m - 1, 0, 0);

    std::cout << answer << '\n';
    return 0;
}
