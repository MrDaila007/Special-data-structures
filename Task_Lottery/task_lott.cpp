#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

using int64 = long long;

static int popcount64(std::uint64_t value) {
#if defined(__GNUG__) || defined(__clang__)
    return __builtin_popcountll(static_cast<unsigned long long>(value));
#elif defined(_MSC_VER)
    return static_cast<int>(__popcnt64(static_cast<unsigned __int64>(value)));
#else
    int count = 0;
    while (value != 0) {
        value &= (value - 1);
        ++count;
    }
    return count;
#endif
}

static std::vector<std::pair<int, int64>> enumerate(const std::vector<int64>& arr) {
    const int n = static_cast<int>(arr.size());
    std::vector<std::pair<int, int64>> res;
    const std::uint64_t total_masks = n == 0 ? 1ULL : (1ULL << n);
    res.reserve(total_masks > 0 ? static_cast<std::size_t>(total_masks - 1) : 0);
    for (std::uint64_t mask = 1; mask < total_masks; ++mask) {
        const int bits = popcount64(mask);
        int64 sum = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1ULL << i)) {
                sum += arr[i];
            }
        }
        res.emplace_back(bits, sum);
    }
    return res;
}

int main() {
    if (std::FILE* check = std::fopen("input.txt", "r")) {
        std::fclose(check);
        if (std::freopen("input.txt", "r", stdin) != nullptr) {
            std::FILE* fout = std::freopen("output.txt", "w", stdout);
            (void)fout;
        }
    }
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
