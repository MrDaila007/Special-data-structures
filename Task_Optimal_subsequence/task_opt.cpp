#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long m;
    cin >> n >> m;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    int mid = n / 2;
    vector<long long> left, right;

    for (int mask = 0; mask < (1 << mid); mask++) {
        long long sum = 0;
        for (int i = 0; i < mid; i++)
            if (mask & (1 << i))
                sum = (sum + a[i]) % m;
        left.push_back(sum);
    }

    int rightSize = n - mid;
    for (int mask = 0; mask < (1 << rightSize); mask++) {
        long long sum = 0;
        for (int i = 0; i < rightSize; i++)
            if (mask & (1 << i))
                sum = (sum + a[mid + i]) % m;
        right.push_back(sum);
    }

    sort(right.begin(), right.end());

    long long ans = 0;
    for (auto x : left) {
        long long need = m - x - 1;
        auto it = upper_bound(right.begin(), right.end(), need);
        if (it != right.begin()) {
            --it;
            ans = max(ans, (x + *it) % m);
        }
        ans = max(ans, (x + right.back()) % m);
    }

    cout << ans << "\n";
    return 0;
}