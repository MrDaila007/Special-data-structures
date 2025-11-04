#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

namespace {

struct NodeStorage {
    std::vector<int> left_child{0};
    std::vector<int> right_child{0};
    std::vector<int> sum{0};

    int clone(int node) {
        left_child.push_back(left_child[node]);
        right_child.push_back(right_child[node]);
        sum.push_back(sum[node]);
        return static_cast<int>(sum.size() - 1);
    }
};

struct PersistentSegmentTree {
    NodeStorage storage;

    int update(int node, int left, int right, int position) {
        int current = storage.clone(node);
        storage.sum[current] = storage.sum[node] + 1;

        if (left != right) {
            int mid = left + (right - left) / 2;
            if (position <= mid) {
                int updated_left = update(storage.left_child[node], left, mid, position);
                storage.left_child[current] = updated_left;
            } else {
                int updated_right = update(storage.right_child[node], mid + 1, right, position);
                storage.right_child[current] = updated_right;
            }
        }

        return current;
    }

    int range_query(int node_r, int node_l, int left, int right, int ql, int qr) const {
        if (qr < left || right < ql || ql > qr) {
            return 0;
        }
        if (ql <= left && right <= qr) {
            return storage.sum[node_r] - storage.sum[node_l];
        }
        int mid = left + (right - left) / 2;
        int left_part = range_query(storage.left_child[node_r], storage.left_child[node_l], left, mid, ql, qr);
        int right_part = range_query(storage.right_child[node_r], storage.right_child[node_l], mid + 1, right, ql, qr);
        return left_part + right_part;
    }
};

struct Query {
    int l;
    int r;
    int x;
    int y;
};

int compute_expected_height(int size) {
    int height = 0;
    int limit = 1;
    while (limit < size) {
        limit <<= 1;
        ++height;
    }
    return height + 1;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifndef _MSC_VER
    if (std::freopen("input.txt", "r", stdin) == nullptr) {
        return 0;
    }
    if (std::freopen("output.txt", "w", stdout) == nullptr) {
        return 0;
    }
#endif

    int n, q;
    if (!(std::cin >> n >> q)) {
        return 0;
    }

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    std::vector<Query> queries(q);
    for (int i = 0; i < q; ++i) {
        std::cin >> queries[i].l >> queries[i].r >> queries[i].x >> queries[i].y;
    }

    std::vector<int> unique_values = a;
    std::sort(unique_values.begin(), unique_values.end());
    unique_values.erase(std::unique(unique_values.begin(), unique_values.end()), unique_values.end());

    const int m = static_cast<int>(unique_values.size());
    const int tree_height = compute_expected_height(std::max(1, m));
    const std::size_t reserve_size = static_cast<std::size_t>(n + 1) * (tree_height + 1);

    PersistentSegmentTree tree;
    tree.storage.left_child.reserve(reserve_size);
    tree.storage.right_child.reserve(reserve_size);
    tree.storage.sum.reserve(reserve_size);
    std::vector<int> roots(n + 1, 0);

    for (int i = 0; i < n; ++i) {
        int pos = static_cast<int>(std::lower_bound(unique_values.begin(), unique_values.end(), a[i]) - unique_values.begin());
        roots[i + 1] = tree.update(roots[i], 0, m - 1, pos);
    }

    std::vector<int> answers;
    answers.reserve(q);

    for (const auto& query : queries) {
        int left_index = static_cast<int>(std::lower_bound(unique_values.begin(), unique_values.end(), query.x) - unique_values.begin());
        int right_index = static_cast<int>(std::upper_bound(unique_values.begin(), unique_values.end(), query.y) - unique_values.begin()) - 1;

        if (left_index > right_index || left_index >= m || right_index < 0) {
            answers.push_back(0);
            continue;
        }

        left_index = std::max(left_index, 0);
        right_index = std::min(right_index, m - 1);

        if (left_index > right_index) {
            answers.push_back(0);
            continue;
        }

        int res = tree.range_query(roots[query.r], roots[query.l - 1], 0, m - 1, left_index, right_index);
        answers.push_back(res);
    }

    for (int ans : answers) {
        std::cout << ans << '\n';
    }

    return 0;
}
