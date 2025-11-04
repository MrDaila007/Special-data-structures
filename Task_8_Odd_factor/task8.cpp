#include <cstdio>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

int main() {
    if (std::freopen("oddfactor.in", "r", stdin) == nullptr) {
        return 0;
    }
    if (std::freopen("oddfactor.out", "w", stdout) == nullptr) {
        return 0;
    }

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    if (!(std::cin >> n >> m)) {
        return 0;
    }

    std::vector<std::pair<int, int>> edges(m);
    std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
    adj.reserve(n + 1);

    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        edges[i] = {a, b};
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }

    std::vector<int> visited(n + 1, 0);
    std::vector<int> parent(n + 1, -1);
    std::vector<int> parent_edge(n + 1, -1);
    std::vector<int> need(n + 1, 0);

    std::vector<int> answer;
    answer.reserve(m);

    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }

        std::vector<int> order;
        std::vector<int> component;
        std::stack<int> st;

        st.push(start);
        visited[start] = 1;
        parent[start] = -1;
        parent_edge[start] = -1;

        while (!st.empty()) {
            int v = st.top();
            st.pop();

            order.push_back(v);
            component.push_back(v);

            for (const auto &edge_info : adj[v]) {
                int to = edge_info.first;
                int id = edge_info.second;
                if (parent[v] == to) {
                    continue;
                }
                if (!visited[to]) {
                    visited[to] = 1;
                    parent[to] = v;
                    parent_edge[to] = id;
                    st.push(to);
                }
            }
        }

        if (component.size() % 2 == 1) {
            std::cout << -1 << '\n';
            return 0;
        }

        for (int v : component) {
            need[v] = 1;
        }

        for (int idx = static_cast<int>(order.size()) - 1; idx >= 0; --idx) {
            int v = order[idx];
            int p = parent[v];
            if (p == -1) {
                continue;
            }
            if (need[v]) {
                answer.push_back(parent_edge[v]);
                need[v] ^= 1;
                need[p] ^= 1;
            }
        }

        if (need[start]) {
            std::cout << -1 << '\n';
            return 0;
        }
    }

    std::cout << answer.size() << '\n';
    for (int id : answer) {
        std::cout << edges[id].first << ' ' << edges[id].second << '\n';
    }

    return 0;
}
