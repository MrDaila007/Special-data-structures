// Deque with constant-time minimum using two min-stacks. Amortized O(1) per operation.
#include <algorithm>
#include <cstdio>
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
    std::vector<int> values;
    std::vector<int> mins;

    inline void push(int x) {
        const int current_min = mins.empty() ? x : std::min(x, mins.back());
        values.push_back(x);
        mins.push_back(current_min);
    }

    inline void pop() {
        values.pop_back();
        mins.pop_back();
    }

    [[nodiscard]] inline int top() const {
        return values.back();
    }

    [[nodiscard]] inline int min() const {
        return mins.back();
    }

    [[nodiscard]] inline bool empty() const {
        return values.empty();
    }

    int size() const {
        return static_cast<int>(values.size());
    }
};

struct MinQueue {
    MinStack front;
    MinStack back;

    void push_back(int x) {
        back.push(x);
    }

    void push_front(int x) {
        front.push(x);
    }

    void pop_back() {
        if (!back.empty()) {
            back.pop();
            return;
        }
        if (front.size() == 1) {
            front.pop();
            return;
        }
        balance();
        back.pop();
    }

    void pop_front() {
        if (!front.empty()) {
            front.pop();
            return;
        }
        if (back.size() == 1) {
            back.pop();
            return;   
        }
        balance();
        front.pop();
    }

    void balance() {
        if (front.empty() && back.empty()) {
            std::abort();
        }
        if (front.empty()) {
            balance(back, front);
        } else{
            balance(front, back);
        }
    }

    void balance(MinStack& source, MinStack& target) {
        const int total_size = source.size();
        const int half_size = total_size / 2;

        MinStack temp;
        temp.values.reserve(static_cast<std::size_t>(half_size));
        temp.mins.reserve(static_cast<std::size_t>(half_size));
        for (int i = 0; i < half_size; ++i) {
            temp.push(source.top());
            source.pop();
        }
        while (!source.empty()) {
            target.push(source.top());
            source.pop();
        }
        while (!temp.empty()) {
            source.push(temp.top());
            temp.pop();
        }
    }

    [[nodiscard]] int min() const {
        if (front.empty()) {
            return back.min();
        }
        if (back.empty()) {
            return front.min();
        }
        return std::min(front.min(), back.min());
    }
};

static inline void balance(MinStack& left, MinStack& right) {
    if (!left.empty() || right.empty()) {
        return;
    }
    while (!left.empty()) {
        right.push(left.top());
        left.pop();
    }
}

int main() {
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

    std::vector<char> output;
    output.reserve(static_cast<std::size_t>(q) * 12);

    auto append_number = [&output](int value) {
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
            const int x = static_cast<int>(input.readLong());
            if (side == 'L') {
                front_stack.push(x);
            } else {
                back_stack.push(x);
            }
        } else {  // '-'
            if (side == 'L') {
                balance(back_stack, front_stack);
                front_stack.pop();
            } else {
                balance(front_stack, back_stack);
                back_stack.pop();
            }
        }

        int current_min;
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

    if (!output.empty()) {
        std::fwrite(output.data(), 1, output.size(), stdout);
    }

    return 0;
}
