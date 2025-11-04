
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) {
        return 0;
    }

    std::vector<std::vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    for (auto& neighbours : graph) {
        std::sort(neighbours.begin(), neighbours.end());
    }

    std::vector<int> order;
    order.reserve(2 * n - 1);

    std::vector<int> stack;
    stack.reserve(2 * n);
    std::vector<int> parent(n + 1, 0);
    std::vector<std::size_t> next_child(n + 1, 0);

    const int root = 1;
    stack.push_back(root);
    parent[root] = 0;

    while (!stack.empty()) {
        int v = stack.back();
        if (next_child[v] == 0) {
            order.push_back(v);
        }

        if (next_child[v] < graph[v].size()) {
            int to = graph[v][next_child[v]++];
            if (to == parent[v]) {
                continue;
            }
            parent[to] = v;
            stack.push_back(to);
        } else {
            stack.pop_back();
            if (!stack.empty()) {
                order.push_back(stack.back());
            }
        }
    }

    for (std::size_t i = 0; i < order.size(); ++i) {
        if (i) {
            std::cout << ' ';
        }
        std::cout << order[i];
    }
    std::cout << '\n';

    return 0;
}
