#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    if (!(cin >> q)) {
        return 0;
    }

    vector<pair<long long, long long>> left_stack;
    vector<pair<long long, long long>> right_stack;

    auto current_min = [&]() -> long long {
        if (left_stack.empty() && right_stack.empty()) {
            return -1;
        }
        if (left_stack.empty()) {
            return right_stack.back().second;
        }
        if (right_stack.empty()) {
            return left_stack.back().second;
        }
        return min(left_stack.back().second, right_stack.back().second);
    };

    auto push_front = [&](long long value) {
        long long new_min = value;
        if (!left_stack.empty()) {
            new_min = min(new_min, left_stack.back().second);
        }
        left_stack.emplace_back(value, new_min);
    };

    auto push_back = [&](long long value) {
        long long new_min = value;
        if (!right_stack.empty()) {
            new_min = min(new_min, right_stack.back().second);
        }
        right_stack.emplace_back(value, new_min);
    };

    auto move_right_to_left = [&]() {
        while (!right_stack.empty()) {
            long long val = right_stack.back().first;
            right_stack.pop_back();
            long long new_min = left_stack.empty() ? val : min(val, left_stack.back().second);
            left_stack.emplace_back(val, new_min);
        }
    };

    auto move_left_to_right = [&]() {
        while (!left_stack.empty()) {
            long long val = left_stack.back().first;
            left_stack.pop_back();
            long long new_min = right_stack.empty() ? val : min(val, right_stack.back().second);
            right_stack.emplace_back(val, new_min);
        }
    };

    auto pop_front = [&]() {
        if (left_stack.empty()) {
            move_right_to_left();
        }
        if (!left_stack.empty()) {
            left_stack.pop_back();
            return;
        }
    };

    auto pop_back = [&]() {
        if (right_stack.empty()) {
            move_left_to_right();
        }
        if (!right_stack.empty()) {
            right_stack.pop_back();
            return;
        }
    };

    for (int i = 0; i < q; ++i) {
        char op;
        cin >> op;
        if (op == '+') {
            char direction;
            long long x;
            cin >> direction >> x;
            if (direction == 'L') {
                push_front(x);
            } else {
                push_back(x);
            }
        } else if (op == '-') {
            char direction;
            cin >> direction;
            if (direction == 'L') {
                pop_front();
            } else {
                pop_back();
            }
        }
        cout << current_min() << '\n';
    }

    return 0;
}

/*
№		Вердикт		Очки	Время	Память
 1	►	
OK
Принято	1 из 1	
0,0 с
520 КБ
2		
OK
Принято	1 из 1	
0,0 с
524 КБ
3		
OK
Принято	1 из 1	
0,0 с
524 КБ
4		
OK
Принято	1 из 1	
0,0 с
520 КБ
5		
OK
Принято	1 из 1	
0,0 с
520 КБ
6		
OK
Принято	1 из 1	
0,0 с
524 КБ
7		
OK
Принято	1 из 1	
0,015 с
524 КБ
8		
OK
Принято	1 из 1	
0,0 с
524 КБ
9		
OK
Принято	1 из 1	
0,0 с
524 КБ
10		
OK
Принято	1 из 1	
0,0 с
528 КБ
11		
OK
Принято	1 из 1	
0,0 с
528 КБ
12		
OK
Принято	1 из 1	
0,0 с
520 КБ
13		
OK
Принято	1 из 1	
0,0 с
528 КБ
14		
OK
Принято	1 из 1	
0,0 с
528 КБ
15		
OK
Принято	1 из 1	
0,0 с
528 КБ
16		
OK
Принято	1 из 1	
0,015 с
524 КБ
17		
OK
Принято	1 из 1	
0,0 с
528 КБ
18		
OK
Принято	1 из 1	
0,0 с
524 КБ
19		
OK
Принято	1 из 1	
0,0 с
528 КБ
20		
OK
Принято	1 из 1	
0,0 с
528 КБ
21		
OK
Принято	1 из 1	
0,093 с
532 КБ
22		
OK
Принято	1 из 1	
0,109 с
536 КБ
23		
OK
Принято	1 из 1	
0,982 с
548 КБ
24		
OK
Принято	1 из 1	
0,982 с
560 КБ
25		
OK
Принято	1 из 1	
0,982 с
524 КБ
26		
OK
Принято	1 из 1	
0,982 с
520 КБ
27		
TLE
Нарушен предел времени	0 из 1	
1,06 с
588 КБ
28		
TLE
Нарушен предел времени	0 из 1	
1,092 с
31 812 КБ
29		
TLE
Нарушен предел времени	0 из 1	
1,06 с
556 КБ
30		
TLE
Нарушен предел времени	0 из 1	
1,045 с
772 КБ
31		
TLE
Нарушен предел времени	0 из 1	
1,107 с
22 524 КБ
32		
TLE
Нарушен предел времени	0 из 1	
1,06 с
564 КБ
33		
TLE
Нарушен предел времени	0 из 1	
1,045 с
520 КБ
34		
TLE
Нарушен предел времени	0 из 1	
1,045 с
848 КБ
35		
TLE
Нарушен предел времени	0 из 1	
1,107 с
31 812 КБ

*/