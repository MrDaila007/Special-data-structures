#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Query {
    int l;
    int r;
    long long h;
    int idx;
};

class Fenwick {
public:
    explicit Fenwick(int size) : n(size), bit(size + 1, 0) {}

    void add(int index, long long delta) {
        for (; index <= n; index += index & -index) {
            bit[index] += delta;
        }
    }

    long long sum(int index) const {
        long long result = 0;
        for (; index > 0; index -= index & -index) {
            result += bit[index];
        }
        return result;
    }

    long long rangeSum(int left, int right) const {
        if (left > right) {
            return 0;
        }
        return sum(right) - sum(left - 1);
    }

private:
    int n;
    vector<long long> bit;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) {
        return 0;
    }

    vector<long long> heights(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> heights[i];
    }

    vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        prefix[i] = prefix[i - 1] + heights[i];
    }

    vector<pair<long long, int>> sortedColumns;
    sortedColumns.reserve(n);
    for (int i = 1; i <= n; ++i) {
        sortedColumns.emplace_back(heights[i], i);
    }
    sort(sortedColumns.begin(), sortedColumns.end());

    vector<Query> queries(q);
    for (int i = 0; i < q; ++i) {
        int l, r;
        long long h;
        cin >> l >> r >> h;
        queries[i] = {l, r, h, i};
    }

    sort(queries.begin(), queries.end(), [](const Query& lhs, const Query& rhs) {
        return lhs.h < rhs.h;
    });

    Fenwick bitCount(n);
    Fenwick bitSum(n);
    vector<long long> answers(q, 0);

    size_t pointer = 0;
    for (const auto& query : queries) {
        while (pointer < sortedColumns.size() && sortedColumns[pointer].first < query.h) {
            int index = sortedColumns[pointer].second;
            long long value = sortedColumns[pointer].first;
            bitCount.add(index, 1);
            bitSum.add(index, value);
            ++pointer;
        }

        long long countLess = bitCount.rangeSum(query.l, query.r);
        long long sumLess = bitSum.rangeSum(query.l, query.r);

        long long totalSum = prefix[query.r] - prefix[query.l - 1];
        long long length = static_cast<long long>(query.r - query.l + 1);

        long long sumGreaterOrEqual = totalSum - sumLess;
        long long countGreaterOrEqual = length - countLess;

        long long costDecrease = sumGreaterOrEqual - query.h * countGreaterOrEqual;
        long long costIncrease = query.h * countLess - sumLess;
        answers[query.idx] = costDecrease + costIncrease;
    }

    for (const auto& ans : answers) {
        cout << ans << '\n';
    }

    return 0;
}
