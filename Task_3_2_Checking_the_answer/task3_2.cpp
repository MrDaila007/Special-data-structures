#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<int> seq;
    seq.reserve(200000);

    int value;
    while (std::cin >> value) {
        seq.push_back(value);
    }

    if (seq.empty() || (seq.size() & 1) == 0) {
        std::cout << "NO\n";
        return 0;
    }

    const int k = static_cast<int>(seq.size());
    const int n = (k + 1) / 2;

    auto inRange = [n](int v) {
        return v >= 1 && v <= n;
    };

    if (!inRange(seq.front()) || seq.front() != seq.back()) {
        std::cout << "NO\n";
        return 0;
    }

    std::vector<char> seen(n + 1, false);
    std::vector<int> parent(n + 1, -1);
    std::vector<int> stack;
    stack.reserve(n);

    const int root = seq.front();
    seen[root] = true;
    stack.push_back(root);
    int seenCount = 1;

    for (int i = 1; i < k; ++i) {
        const int current = stack.back();
        const int next = seq[i];

        if (!inRange(next) || next == current) {
            std::cout << "NO\n";
            return 0;
        }

        if (!seen[next]) {
            if (seenCount == n) {
                std::cout << "NO\n";
                return 0;
            }
            seen[next] = true;
            ++seenCount;
            parent[next] = current;
            stack.push_back(next);
        } else {
            if (stack.size() < 2 || parent[current] != next) {
                std::cout << "NO\n";
                return 0;
            }
            stack.pop_back();
        }
    }

    if (stack.size() != 1 || stack.back() != root || seenCount != n) {
        std::cout << "NO\n";
        return 0;
    }

    std::cout << "YES\n";
    return 0;
}
