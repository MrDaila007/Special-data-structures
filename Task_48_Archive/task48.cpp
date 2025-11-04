// Solution for "Task 48. Archive"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

struct Node {
    int value;
    int priority;
    int size;
    Node* left;
    Node* right;

    Node(int v, int p) : value(v), priority(p), size(1), left(nullptr), right(nullptr) {}
};

int get_size(Node* node) {
    return node ? node->size : 0;
}

void update(Node* node) {
    if (node) {
        node->size = 1 + get_size(node->left) + get_size(node->right);
    }
}

std::pair<Node*, Node*> split(Node* root, int left_size) {
    if (!root) {
        return {nullptr, nullptr};
    }
    if (get_size(root->left) >= left_size) {
        auto splitted = split(root->left, left_size);
        root->left = splitted.second;
        update(root);
        return {splitted.first, root};
    }
    auto splitted = split(root->right, left_size - get_size(root->left) - 1);
    root->right = splitted.first;
    update(root);
    return {root, splitted.second};
}

Node* merge(Node* left, Node* right) {
    if (!left || !right) {
        return left ? left : right;
    }
    if (left->priority < right->priority) {
        left->right = merge(left->right, right);
        update(left);
        return left;
    }
    right->left = merge(left, right->left);
    update(right);
    return right;
}

void collect(Node* root, std::vector<int>& result) {
    if (!root) {
        return;
    }
    collect(root->left, result);
    result.push_back(root->value);
    collect(root->right, result);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream fin("archive.in");
    if (!fin.is_open()) {
        return 0;
    }
    std::ofstream fout("archive.out");

    int n = 0;
    int q = 0;
    fin >> n >> q;
    if (n <= 0) {
        fout.close();
        return 0;
    }

    std::mt19937 rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

    Node* root = nullptr;
    for (int i = 1; i <= n; ++i) {
        Node* node = new Node(i, static_cast<int>(rng()));
        root = merge(root, node);
    }

    for (int i = 0; i < q; ++i) {
        int a = 0;
        int b = 0;
        fin >> a >> b;
        if (a < 1 || b < a || b > n) {
            continue;
        }

        auto left_split = split(root, b);
        auto left_part = left_split.first;
        Node* right_part = left_split.second;

        auto right_split = split(left_part, a - 1);
        Node* prefix = right_split.first;
        Node* segment = right_split.second;

        root = merge(segment, merge(prefix, right_part));
    }

    std::vector<int> order;
    order.reserve(n);
    collect(root, order);

    for (size_t i = 0; i < order.size(); ++i) {
        if (i) {
            fout << ' ';
        }
        fout << order[i];
    }
    fout << '\n';

    fout.close();
    return 0;
}
