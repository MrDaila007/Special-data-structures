
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) {
        return 0;
    }

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    const int block_size = static_cast<int>(sqrt(n)) + 1;
    const int block_count = (n + block_size - 1) / block_size;

    vector<int> block_id(n + 1);
    for (int i = 1; i <= n; ++i) {
        block_id[i] = (i - 1) / block_size;
    }

    vector<int> to(n + 1), jump(n + 1), last(n + 1);

    auto rebuild_block = [&](int b) {
        int left = b * block_size + 1;
        int right = min(n, (b + 1) * block_size);
        for (int i = right; i >= left; --i) {
            int next = i + a[i];
            if (next > right) {
                to[i] = next;
                jump[i] = 1;
                last[i] = i;
            } else {
                to[i] = to[next];
                jump[i] = jump[next] + 1;
                last[i] = last[next];
            }
        }
    };

    for (int b = 0; b < block_count; ++b) {
        rebuild_block(b);
    }

    while (m--) {
        int type;
        cin >> type;
        if (type == 0) {
            int pos, val;
            cin >> pos >> val;
            a[pos] = val;
            rebuild_block(block_id[pos]);
        } else {
            int pos;
            cin >> pos;
            int jumps_total = 0;
            int last_hole = pos;
            while (pos <= n) {
                jumps_total += jump[pos];
                last_hole = last[pos];
                if (to[pos] > n) {
                    break;
                }
                pos = to[pos];
            }
            cout << last_hole << ' ' << jumps_total << '\n';
        }
    }

    return 0;
}
