// Double-ended queue with logarithmic-time minimum using frequency-balanced map.
#include <deque>
#include <iostream>
#include <map>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    if (!(std::cin >> q)) {
        return 0;
    }

    std::deque<long long> dq;
    std::map<long long, int> freq;

    std::string output;
    output.reserve(static_cast<std::size_t>(q) * 12);

    for (int i = 0; i < q; ++i) {
        std::string op;
        std::cin >> op;

        if (op == "+L") {
            long long x;
            std::cin >> x;
            dq.push_front(x);
            ++freq[x];
        } else if (op == "+R") {
            long long x;
            std::cin >> x;
            dq.push_back(x);
            ++freq[x];
        } else if (op == "-L") {
            if (!dq.empty()) {
                const long long value = dq.front();
                dq.pop_front();
                auto it = freq.find(value);
                if (--(it->second) == 0) {
                    freq.erase(it);
                }
            }
        } else if (op == "-R") {
            if (!dq.empty()) {
                const long long value = dq.back();
                dq.pop_back();
                auto it = freq.find(value);
                if (--(it->second) == 0) {
                    freq.erase(it);
                }
            }
        }

        const long long current_min = freq.empty() ? -1 : freq.begin()->first;
        output.append(std::to_string(current_min));
        output.push_back('\n');
    }

    std::cout << output;

    return 0;
}
