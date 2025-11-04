
#include <cstdio>
#include <iostream>
#include <vector>

class Fenwick {
public:
    explicit Fenwick(int size) : tree(size + 1, 0) {}

    void add(int index, int delta) {
        for (int i = index; i < static_cast<int>(tree.size()); i += i & -i) {
            tree[i] += delta;
        }
    }

    int sum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree[i];
        }
        return result;
    }

private:
    std::vector<int> tree;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (std::freopen("input.txt", "r", stdin) == nullptr) {
        return 0;
    }
    if (std::freopen("output.txt", "w", stdout) == nullptr) {
        return 0;
    }

    int n;
    if (!(std::cin >> n)) {
        return 0;
    }

    const int MAX_COORD = 500000;
    Fenwick fenwick(MAX_COORD + 2);  // extra space for 1-based indexing and upper bound
    std::vector<int> levelCount(n, 0);

    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        (void)y;

        int idx = x + 1;  // shift to 1-based index
        int level = fenwick.sum(idx);
        ++levelCount[level];
        fenwick.add(idx, 1);
    }

    for (int level = 0; level < n; ++level) {
        std::cout << levelCount[level] << '\n';
    }

    return 0;
}
