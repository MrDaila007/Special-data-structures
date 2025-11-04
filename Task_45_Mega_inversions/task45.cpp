#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

class Fenwick {
public:
    explicit Fenwick(int size) : tree_(size + 1, 0) {}

    void add(int index, int delta) {
        for (int i = index; i < static_cast<int>(tree_.size()); i += i & -i) {
            tree_[i] += delta;
        }
    }

    int sum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree_[i];
        }
        return result;
    }

    int size() const {
        return static_cast<int>(tree_.size()) - 1;
    }

private:
    std::vector<int> tree_;
};

}  // namespace

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

    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> permutation[i];
    }

    Fenwick left_tree(n);
    std::vector<int64_t> left_greater(n, 0);
    for (int i = 0; i < n; ++i) {
        const int value = permutation[i];
        const int not_greater = left_tree.sum(value);
        left_greater[i] = i - static_cast<int64_t>(not_greater);
        left_tree.add(value, 1);
    }

    Fenwick right_tree(n);
    std::vector<int64_t> right_smaller(n, 0);
    for (int i = n - 1; i >= 0; --i) {
        const int value = permutation[i];
        right_smaller[i] = right_tree.sum(value - 1);
        right_tree.add(value, 1);
    }

    int64_t answer = 0;
    for (int i = 0; i < n; ++i) {
        answer += left_greater[i] * right_smaller[i];
    }

    std::cout << answer << '\n';
    return 0;
}
