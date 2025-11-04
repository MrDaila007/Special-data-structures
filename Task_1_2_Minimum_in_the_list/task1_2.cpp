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

    std::vector<long long> buffer;
    buffer.reserve(source.size());
    while (!source.empty()) {
        buffer.push_back(source.top());
        source.pop();
    }
    for (long long value : buffer) {
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

        if (front_stack.empty() && back_stack.empty()) {
            std::cout << -1;
        } else if (front_stack.empty()) {
            std::cout << back_stack.min();
        } else if (back_stack.empty()) {
            std::cout << front_stack.min();
        } else {
            std::cout << std::min(front_stack.min(), back_stack.min());
        }
        std::cout << '\n';
    }

    return 0;
}
