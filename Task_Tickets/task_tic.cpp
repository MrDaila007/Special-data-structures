// Task: Tickets - process seat reservation queries on a railway line.
// Solution: range maximum segment tree with lazy propagation.

#include <iostream>
#include <vector>
#include <limits>

class SegmentTree {
public:
    void init(int n) {
        size_ = 1;
        while (size_ < n) {
            size_ <<= 1;
        }
        tree_.assign(size_ * 2, 0);
        lazy_.assign(size_ * 2, 0);
    }

    void range_add(int l, int r, int value) {
        if (l > r) {
            return;
        }
        range_add(l, r, value, 1, 0, size_ - 1);
    }

    int range_max(int l, int r) {
        if (l > r) {
            return 0;
        }
        return range_max(l, r, 1, 0, size_ - 1);
    }

private:
    int size_{0};
    std::vector<int> tree_;
    std::vector<int> lazy_;

    void apply(int node, int value) {
        tree_[node] += value;
        lazy_[node] += value;
    }

    void push(int node) {
        if (lazy_[node] != 0 && node < size_) {
            apply(node * 2, lazy_[node]);
            apply(node * 2 + 1, lazy_[node]);
            lazy_[node] = 0;
        }
    }

    void range_add(int l, int r, int value, int node, int lx, int rx) {
        if (lx > r || rx < l) {
            return;
        }
        if (l <= lx && rx <= r) {
            apply(node, value);
            return;
        }
        push(node);
        int mid = (lx + rx) >> 1;
        range_add(l, r, value, node * 2, lx, mid);
        range_add(l, r, value, node * 2 + 1, mid + 1, rx);
        tree_[node] = std::max(tree_[node * 2], tree_[node * 2 + 1]);
    }

    int range_max(int l, int r, int node, int lx, int rx) {
        if (lx > r || rx < l) {
            return std::numeric_limits<int>::min();
        }
        if (l <= lx && rx <= r) {
            return tree_[node];
        }
        push(node);
        int mid = (lx + rx) >> 1;
        int left = range_max(l, r, node * 2, lx, mid);
        int right = range_max(l, r, node * 2 + 1, mid + 1, rx);
        return std::max(left, right);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    int m = 0;
    if (!(std::cin >> n >> k >> m)) {
        return 0;
    }

    const int segments = n > 0 ? n - 1 : 0;
    SegmentTree tree;
    tree.init(std::max(segments, 1));

    for (int i = 0; i < m; ++i) {
        int x = 0;
        int y = 0;
        std::cin >> x >> y;
        int l = x;
        int r = y - 1;

        if (segments <= 0 || l > r) {
            std::cout << 0 << '\n';
            continue;
        }

        int current_max = tree.range_max(l, r);
        if (current_max >= k) {
            std::cout << 0 << '\n';
        } else {
            tree.range_add(l, r, 1);
            std::cout << 1 << '\n';
        }
    }

    return 0;
}
