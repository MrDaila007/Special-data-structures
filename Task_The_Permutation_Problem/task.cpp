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

    int prefix_sum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree[i];
        }
        return result;
    }

    int find_by_order(int order) const {
        int idx = 0;
        int bit = 1;
        while (bit < static_cast<int>(tree.size())) {
            bit <<= 1;
        }
        for (int step = bit; step > 0; step >>= 1) {
            int next = idx + step;
            if (next < static_cast<int>(tree.size()) && tree[next] < order) {
                order -= tree[next];
                idx = next;
            }
        }
        return idx + 1;
    }

private:
    std::vector<int> tree;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) {
        return 0;
    }

    std::vector<int> inversions(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> inversions[i];
    }

    Fenwick fenwick(n);
    for (int value = 1; value <= n; ++value) {
        fenwick.add(value, 1);
    }

    std::vector<int> permutation(n, 0);
    for (int i = n - 1; i >= 0; --i) {
        int inv_count = inversions[i];
        int remaining = i + 1;
        if (inv_count < 0 || inv_count >= remaining) {
            std::cout << -1 << '\n';
            return 0;
        }
        int order = remaining - inv_count;
        int value = fenwick.find_by_order(order);
        permutation[i] = value;
        fenwick.add(value, -1);
    }

    for (int i = 0; i < n; ++i) {
        if (i) {
            std::cout << ' ';
        }
        std::cout << permutation[i];
    }
    std::cout << '\n';

    return 0;
}
