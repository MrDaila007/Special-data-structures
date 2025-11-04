
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    long long sum_even = 0;
    long long sum_odd = 0;
    int cnt_even = 0;
    int cnt_odd = 0;
    long long lazy = 0;
};

class SegmentTree {
public:
    explicit SegmentTree(const std::vector<long long>& data) {
        n = static_cast<int>(data.size());
        tree.resize(4 * n);
        build(1, 0, n - 1, data);
    }

    void pointAssign(int pos, long long value) {
        pointAssign(1, 0, n - 1, pos, value);
    }

    void rangeIncrement(int l, int r, long long value) {
        rangeIncrement(1, 0, n - 1, l, r, value);
    }

    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

private:
    int n = 0;
    std::vector<Node> tree;

    static Node merge(const Node& left, const Node& right) {
        Node res;
        res.sum_even = left.sum_even + right.sum_even;
        res.sum_odd = left.sum_odd + right.sum_odd;
        res.cnt_even = left.cnt_even + right.cnt_even;
        res.cnt_odd = left.cnt_odd + right.cnt_odd;
        return res;
    }

    void apply(int v, long long value) {
        if (value == 0) {
            return;
        }

        Node& node = tree[v];
        long long even_sum = node.sum_even;
        long long odd_sum = node.sum_odd;
        long long even_cnt = node.cnt_even;
        long long odd_cnt = node.cnt_odd;

        if ((value & 1LL) == 0) {
            node.sum_even += even_cnt * value;
            node.sum_odd += odd_cnt * value;
        } else {
            node.sum_even = odd_sum + odd_cnt * value;
            node.sum_odd = even_sum + even_cnt * value;
            node.cnt_even = static_cast<int>(odd_cnt);
            node.cnt_odd = static_cast<int>(even_cnt);
        }

        if ((value & 1LL) == 0) {
            // counts stay the same
        }

        node.lazy += value;
    }

    void push(int v) {
        long long pending = tree[v].lazy;
        if (pending != 0) {
            apply(v * 2, pending);
            apply(v * 2 + 1, pending);
            tree[v].lazy = 0;
        }
    }

    void build(int v, int tl, int tr, const std::vector<long long>& data) {
        if (tl == tr) {
            long long value = data[tl];
            Node& node = tree[v];
            if (value % 2 == 0) {
                node.sum_even = value;
                node.cnt_even = 1;
            } else {
                node.sum_odd = value;
                node.cnt_odd = 1;
            }
            return;
        }

        int tm = (tl + tr) / 2;
        build(v * 2, tl, tm, data);
        build(v * 2 + 1, tm + 1, tr, data);
        tree[v] = merge(tree[v * 2], tree[v * 2 + 1]);
    }

    void pointAssign(int v, int tl, int tr, int pos, long long value) {
        if (tl == tr) {
            Node& node = tree[v];
            node = Node();
            if (value % 2 == 0) {
                node.sum_even = value;
                node.cnt_even = 1;
            } else {
                node.sum_odd = value;
                node.cnt_odd = 1;
            }
            return;
        }

        push(v);
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            pointAssign(v * 2, tl, tm, pos, value);
        } else {
            pointAssign(v * 2 + 1, tm + 1, tr, pos, value);
        }
        tree[v] = merge(tree[v * 2], tree[v * 2 + 1]);
    }

    void rangeIncrement(int v, int tl, int tr, int l, int r, long long value) {
        if (l > r) {
            return;
        }
        if (l == tl && r == tr) {
            apply(v, value);
            return;
        }
        push(v);
        int tm = (tl + tr) / 2;
        rangeIncrement(v * 2, tl, tm, l, std::min(r, tm), value);
        rangeIncrement(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, value);
        tree[v] = merge(tree[v * 2], tree[v * 2 + 1]);
    }

    Node query(int v, int tl, int tr, int l, int r) {
        if (l > r) {
            return Node();
        }
        if (l == tl && r == tr) {
            return tree[v];
        }
        push(v);
        int tm = (tl + tr) / 2;
        if (r <= tm) {
            return query(v * 2, tl, tm, l, r);
        }
        if (l > tm) {
            return query(v * 2 + 1, tm + 1, tr, l, r);
        }
        Node left = query(v * 2, tl, tm, l, tm);
        Node right = query(v * 2 + 1, tm + 1, tr, tm + 1, r);
        return merge(left, right);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    if (!(std::cin >> n >> q)) {
        return 0;
    }

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    SegmentTree seg(a);
    std::string output;
    output.reserve(q * 3);

    for (int i = 0; i < q; ++i) {
        int type = 0;
        std::cin >> type;
        if (type == 1) {
            int p = 0;
            long long v = 0;
            std::cin >> p >> v;
            seg.pointAssign(p - 1, v);
        } else if (type == 2) {
            int l = 0;
            int r = 0;
            std::cin >> l >> r;
            seg.rangeIncrement(l - 1, r - 1, 1);
        } else if (type == 3) {
            int l = 0;
            int r = 0;
            std::cin >> l >> r;
            Node res = seg.query(l - 1, r - 1);
            output.append(std::to_string(res.sum_even));
            output.push_back('\n');
        } else if (type == 4) {
            int l = 0;
            int r = 0;
            std::cin >> l >> r;
            Node res = seg.query(l - 1, r - 1);
            output.append(std::to_string(res.sum_odd));
            output.push_back('\n');
        }
    }

    std::cout << output;
    return 0;
}
