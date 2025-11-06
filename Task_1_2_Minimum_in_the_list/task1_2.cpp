// Optimized deque minimum tracker using two min-stacks with amortized O(1) operations.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct MinStack {
    std::vector<long long> values;
    std::vector<long long> mins;

    void push(long long x) {
        long long current_min = mins.empty() ? x : std::min(x, mins.back());
        values.push_back(x);
        mins.push_back(current_min);
    }

    void pop() {
        values.pop_back();
        mins.pop_back();
    }

    [[nodiscard]] long long top() const {
        return values.back();
    }

    [[nodiscard]] long long min() const {
        return mins.back();
    }

    [[nodiscard]] bool empty() const {
        return values.empty();
    }

    [[nodiscard]] std::size_t size() const {
        return values.size();
    }
};

static void transfer(MinStack& source, MinStack& target) {
    if (!target.empty() || source.empty()) {
        return;
    }

    while (!source.empty()) {
        const long long value = source.top();
        source.pop();
        target.push(value);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    if (!(std::cin >> q)) {
        return 0;
    }

    MinStack front_stack;
    MinStack back_stack;
    front_stack.values.reserve(static_cast<std::size_t>(q));
    front_stack.mins.reserve(static_cast<std::size_t>(q));
    back_stack.values.reserve(static_cast<std::size_t>(q));
    back_stack.mins.reserve(static_cast<std::size_t>(q));

    std::string output;
    output.reserve(static_cast<std::size_t>(q) * 12);

    for (int i = 0; i < q; ++i) {
        std::string op;
        std::cin >> op;

        if (op == "+L") {
            long long x;
            std::cin >> x;
            front_stack.push(x);
        } else if (op == "+R") {
            long long x;
            std::cin >> x;
            back_stack.push(x);
        } else if (op == "-L") {
            transfer(back_stack, front_stack);
            front_stack.pop();
        } else if (op == "-R") {
            transfer(front_stack, back_stack);
            back_stack.pop();
        }

        long long current_min = -1;
        if (front_stack.empty() && back_stack.empty()) {
            current_min = -1;
        } else if (front_stack.empty()) {
            current_min = back_stack.min();
        } else if (back_stack.empty()) {
            current_min = front_stack.min();
        } else {
            current_min = std::min(front_stack.min(), back_stack.min());
        }
        output.append(std::to_string(current_min));
        output.push_back('\n');
    }

    std::cout << output;

    return 0;
}
