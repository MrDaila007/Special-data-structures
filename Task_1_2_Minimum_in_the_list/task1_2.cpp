// Deque with constant-time minimum using two min-stacks. Amortized O(1) per operation.
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

struct FastInput {
    static constexpr std::size_t BUF_SIZE = 1 << 16;
    char data[BUF_SIZE];
    std::size_t idx = 0;
    std::size_t len = 0;

    inline char readRaw() {
        if (idx >= len) {
            len = std::fread(data, 1, BUF_SIZE, stdin);
            idx = 0;
            if (len == 0) {
                return '\0';
            }
        }
        return data[idx++];
    }

    inline char readChar() {
        char c = readRaw();
        while (c <= ' ' && c != '\0') {
            c = readRaw();
        }
        return c;
    }

    inline long long readLong() {
        char c = readChar();
        long long sign = 1;
        if (c == '-') {
            sign = -1;
            c = readRaw();
        }
        long long value = 0;
        while (c > ' ') {
            value = value * 10 + (c - '0');
            c = readRaw();
        }
        return sign * value;
    }
};

struct MinStack {
    std::vector<long long> values;
    std::vector<long long> mins;

    inline void push(long long x) {
        const long long current_min = mins.empty() ? x : std::min(x, mins.back());
        values.push_back(x);
        mins.push_back(current_min);
    }

    inline void pop() {
        values.pop_back();
        mins.pop_back();
    }

    [[nodiscard]] inline long long top() const {
        return values.back();
    }

    [[nodiscard]] inline long long min() const {
        return mins.back();
    }

    [[nodiscard]] inline bool empty() const {
        return values.empty();
    }
};

static inline void transfer(MinStack& source, MinStack& target) {
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

    FastInput input;

    int q = static_cast<int>(input.readLong());
    if (q <= 0) {
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

    auto append_number = [&output](long long value) {
        if (value == 0) {
            output.push_back('0');
            return;
        }
        if (value < 0) {
            output.push_back('-');
            value = -value;
        }
        char buffer[20];
        int idx = 0;
        while (value > 0) {
            buffer[idx++] = static_cast<char>('0' + (value % 10));
            value /= 10;
        }
        while (idx-- > 0) {
            output.push_back(buffer[idx]);
        }
    };

    for (int i = 0; i < q; ++i) {
        const char op = input.readChar();
        const char side = input.readChar();

        if (op == '+') {
            const long long x = input.readLong();
            if (side == 'L') {
                front_stack.push(x);
            } else {
                back_stack.push(x);
            }
        } else {  // '-'
            if (side == 'L') {
                transfer(back_stack, front_stack);
                front_stack.pop();
            } else {
                transfer(front_stack, back_stack);
                back_stack.pop();
            }
        }

        long long current_min;
        if (front_stack.empty()) {
            current_min = back_stack.empty() ? -1 : back_stack.min();
        } else if (back_stack.empty()) {
            current_min = front_stack.min();
        } else {
            current_min = std::min(front_stack.min(), back_stack.min());
        }
        append_number(current_min);
        output.push_back('\n');
    }

    std::cout << output;

    return 0;
}
