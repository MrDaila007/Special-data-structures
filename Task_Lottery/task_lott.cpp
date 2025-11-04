#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using int64 = long long;

static std::vector<std::pair<int, int64>> enumerate(const std::vector<int64>& arr) {
    const int n = static_cast<int>(arr.size());
    std::vector<std::pair<int, int64>> res;
    res.reserve(1 << n);
    for (int mask = 1; mask < (1 << n); ++mask) {
        int bits = __builtin_popcount(static_cast<unsigned>(mask));
        int64 sum = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                sum += arr[i];
            }
        }
        res.emplace_back(bits, sum);
    }
    return res;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) {
        return 0;
    }
    std::vector<int64> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    int64 L, R;
    std::cin >> L >> R;

    const int mid = n / 2;
    std::vector<int64> left(a.begin(), a.begin() + mid);
    std::vector<int64> right(a.begin() + mid, a.end());

    auto leftSub = enumerate(left);
    auto rightSub = enumerate(right);

    std::unordered_map<int, std::vector<int64>> rightGroups;
    rightGroups.reserve(rightSub.size() * 2 + 1);
    for (const auto& [cnt, sum] : rightSub) {
        rightGroups[cnt].push_back(sum);
    }
    for (auto& [cnt, vec] : rightGroups) {
        std::sort(vec.begin(), vec.end());
    }

    int64 answer = 0;

    auto countRight = [&](int needCnt, int64 minSum, int64 maxSum) -> int64 {
        auto it = rightGroups.find(needCnt);
        if (it == rightGroups.end()) {
            return 0;
        }
        const auto& vec = it->second;
        auto lower = std::lower_bound(vec.begin(), vec.end(), minSum);
        auto upper = std::upper_bound(vec.begin(), vec.end(), maxSum);
        return static_cast<int64>(upper - lower);
    };

    for (const auto& [cntL, sumL] : leftSub) {
        int64 minSum = L * cntL;
        int64 maxSum = R * cntL;
        if (cntL > 0 && sumL >= minSum && sumL <= maxSum) {
            ++answer;
        }
        for (const auto& [cntR, vec] : rightGroups) {
            int total = cntL + cntR;
            int64 minTotal = static_cast<int64>(L) * total;
            int64 maxTotal = static_cast<int64>(R) * total;
            int64 remainingMin = minTotal - sumL;
            int64 remainingMax = maxTotal - sumL;
            answer += countRight(cntR, remainingMin, remainingMax);
        }
    }

    for (const auto& [cnt, sum] : rightSub) {
        if (cnt > 0) {
            int64 minSum = L * cnt;
            int64 maxSum = R * cnt;
            if (sum >= minSum && sum <= maxSum) {
                ++answer;
            }
        }
    }

    std::cout << answer << '\n';
    return 0;
}
