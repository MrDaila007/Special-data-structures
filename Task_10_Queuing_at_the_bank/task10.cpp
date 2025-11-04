// Solution for "Очередь в банк" task.
// Maintains schedule using an implicit treap that aggregates total visit duration
// and minimal start time needed to avoid waiting. Each query can then be answered
// by retrieving prefix statistics in logarithmic time.

#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

using namespace std;

struct Node {
    int key;               // arrival time t
    int duration;          // visit duration d
    uint32_t priority;
    Node* left;
    Node* right;
    long long sum_duration; // total duration in subtree
    long long min_start;    // max over t_i - prefix_durations_before_i
};

mt19937 rng(0xC0FFEE);

Node* make_node(int key, int duration) {
    Node* node = new Node;
    node->key = key;
    node->duration = duration;
    node->priority = rng();
    node->left = node->right = nullptr;
    node->sum_duration = duration;
    node->min_start = key;
    return node;
}

long long get_sum(Node* node) {
    return node ? node->sum_duration : 0LL;
}

void pull(Node* node) {
    if (!node) {
        return;
    }
    long long sum_left = get_sum(node->left);
    long long sum_right = get_sum(node->right);

    node->sum_duration = sum_left + node->duration + sum_right;

    long long best = LLONG_MIN;
    if (node->left) {
        best = max(best, node->left->min_start);
    }

    long long self_value = node->key - sum_left;
    best = max(best, self_value);

    if (node->right) {
        long long adjusted_right = node->right->min_start - (sum_left + node->duration);
        best = max(best, adjusted_right);
    }

    node->min_start = best;
}

Node* merge(Node* left, Node* right) {
    if (!left || !right) {
        return left ? left : right;
    }
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        pull(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        pull(right);
        return right;
    }
}

void split(Node* current, int key, Node*& left, Node*& right) {
    if (!current) {
        left = right = nullptr;
        return;
    }
    if (current->key < key) {
        split(current->right, key, current->right, right);
        left = current;
        pull(left);
    } else {
        split(current->left, key, left, current->left);
        right = current;
        pull(right);
    }
}

Node* insert(Node* root, Node* node) {
    if (!root) {
        return node;
    }
    if (node->priority > root->priority) {
        split(root, node->key, node->left, node->right);
        pull(node);
        return node;
    }
    if (node->key < root->key) {
        root->left = insert(root->left, node);
    } else {
        root->right = insert(root->right, node);
    }
    pull(root);
    return root;
}

Node* erase(Node* root, int key) {
    if (!root) {
        return nullptr;
    }
    if (root->key == key) {
        Node* merged = merge(root->left, root->right);
        delete root;
        return merged;
    }
    if (key < root->key) {
        root->left = erase(root->left, key);
    } else {
        root->right = erase(root->right, key);
    }
    pull(root);
    return root;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    if (!(cin >> q)) {
        return 0;
    }

    struct JoinInfo {
        int t = 0;
        int d = 0;
        bool active = false;
    };

    vector<JoinInfo> joins(q + 1);
    Node* root = nullptr;

    for (int idx = 1; idx <= q; ++idx) {
        char type;
        cin >> type;
        if (type == '+') {
            int t, d;
            cin >> t >> d;
            Node* node = make_node(t, d);
            root = insert(root, node);
            joins[idx] = {t, d, true};
        } else if (type == '-') {
            int ref;
            cin >> ref;
            if (ref >= 1 && ref <= q && joins[ref].active) {
                root = erase(root, joins[ref].t);
                joins[ref].active = false;
            }
        } else if (type == '?') {
            int t;
            cin >> t;
            Node *left, *right;
            split(root, t + 1, left, right); // left contains keys <= t
            long long finish_time = 0;
            if (left) {
                long long start = max(0LL, left->min_start);
                finish_time = start + left->sum_duration;
            }
            long long wait = max(0LL, finish_time - t);
            cout << wait << '\n';
            root = merge(left, right);
        }
    }

    return 0;
}
