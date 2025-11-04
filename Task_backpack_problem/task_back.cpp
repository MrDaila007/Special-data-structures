
#include <iostream>
#include <vector>
#include <algorithm>

struct SubsetState {
    long long weight;
    long long value;
    int mask;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long W;
    if (!(std::cin >> n >> W)) {
        return 0;
    }

    std::vector<long long> weights(n);
    std::vector<long long> values(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> weights[i] >> values[i];
    }

    int n1 = n / 2;
    int n2 = n - n1;

    std::vector<SubsetState> first_half;
    first_half.reserve(1 << n1);

    for (int mask = 0; mask < (1 << n1); ++mask) {
        long long w_sum = 0;
        long long v_sum = 0;
        bool ok = true;
        for (int i = 0; i < n1; ++i) {
            if (mask & (1 << i)) {
                w_sum += weights[i];
                if (w_sum > W) {
                    ok = false;
                    break;
                }
                v_sum += values[i];
            }
        }
        if (ok) {
            first_half.push_back({w_sum, v_sum, mask});
        }
    }

    std::sort(first_half.begin(), first_half.end(), [](const SubsetState& a, const SubsetState& b) {
        if (a.weight != b.weight) {
            return a.weight < b.weight;
        }
        return a.value > b.value;
    });

    std::vector<SubsetState> filtered;
    filtered.reserve(first_half.size());
    long long best_value = -1;
    for (const auto& state : first_half) {
        if (state.value > best_value) {
            filtered.push_back(state);
            best_value = state.value;
        }
    }

    std::vector<long long> filtered_weights;
    filtered_weights.reserve(filtered.size());
    for (const auto& state : filtered) {
        filtered_weights.push_back(state.weight);
    }

    long long best_total = -1;
    int best_mask_first = 0;
    int best_mask_second = 0;

    for (int mask = 0; mask < (1 << n2); ++mask) {
        long long w_sum = 0;
        long long v_sum = 0;
        bool ok = true;
        for (int i = 0; i < n2; ++i) {
            if (mask & (1 << i)) {
                w_sum += weights[n1 + i];
                if (w_sum > W) {
                    ok = false;
                    break;
                }
                v_sum += values[n1 + i];
            }
        }
        if (!ok) {
            continue;
        }

        long long remaining = W - w_sum;
        auto it = std::upper_bound(filtered_weights.begin(), filtered_weights.end(), remaining);
        if (it == filtered_weights.begin()) {
            if (v_sum > best_total) {
                best_total = v_sum;
                best_mask_first = 0;
                best_mask_second = mask;
            }
            continue;
        }

        int idx = static_cast<int>(it - filtered_weights.begin()) - 1;
        const auto& state = filtered[idx];
        long long total = v_sum + state.value;
        if (total > best_total) {
            best_total = total;
            best_mask_first = state.mask;
            best_mask_second = mask;
        }
    }

    std::vector<int> chosen_indices;
    chosen_indices.reserve(n);

    for (int i = 0; i < n1; ++i) {
        if (best_mask_first & (1 << i)) {
            chosen_indices.push_back(i + 1);
        }
    }
    for (int i = 0; i < n2; ++i) {
        if (best_mask_second & (1 << i)) {
            chosen_indices.push_back(n1 + i + 1);
        }
    }

    std::cout << chosen_indices.size() << '\n';
    for (std::size_t i = 0; i < chosen_indices.size(); ++i) {
        if (i) {
            std::cout << ' ';
        }
        std::cout << chosen_indices[i];
    }
    std::cout << '\n';

    return 0;
}
