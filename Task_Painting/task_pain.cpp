
#include <iostream>
#include <map>

struct Segment {
    int r;       // right boundary (exclusive)
    bool black;  // true if the segment is black
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) {
        return 0;
    }

    const int INF = 600000;  // covers all possible coordinates

    std::map<int, Segment> segments;
    segments.emplace(0, Segment{INF, false});

    long long blackSegments = 0;
    long long blackLength = 0;

    auto split = [&](int x) -> std::map<int, Segment>::iterator {
        if (x >= INF) {
            return segments.end();
        }
        auto it = segments.lower_bound(x);
        if (it != segments.end() && it->first == x) {
            return it;
        }
        if (it == segments.begin()) {
            return segments.end();
        }
        --it;
        if (it->second.r <= x) {
            return std::next(it);
        }
        int r = it->second.r;
        bool isBlack = it->second.black;
        it->second.r = x;
        auto inserted = segments.emplace(x, Segment{r, isBlack}).first;
        if (isBlack) {
            ++blackSegments;
        }
        return inserted;
    };

    auto mergeAround = [&](std::map<int, Segment>::iterator it) {
        if (it == segments.end()) {
            return it;
        }
        if (it != segments.begin()) {
            auto prevIt = std::prev(it);
            if (prevIt->second.black == it->second.black && prevIt->second.r == it->first) {
                if (it->second.black) {
                    --blackSegments;
                }
                prevIt->second.r = it->second.r;
                segments.erase(it);
                it = prevIt;
            }
        }
        auto nextIt = std::next(it);
        if (nextIt != segments.end() && nextIt->second.black == it->second.black && it->second.r == nextIt->first) {
            if (it->second.black) {
                --blackSegments;
            }
            it->second.r = nextIt->second.r;
            segments.erase(nextIt);
        }
        return it;
    };

    for (int i = 0; i < n; ++i) {
        char colorChar;
        int start, length;
        std::cin >> colorChar >> start >> length;

        int l = start;
        int r = start + length;
        bool makeBlack = (colorChar == 'B');

        if (length == 0) {
            std::cout << blackSegments << ' ' << blackLength << '\n';
            continue;
        }

        auto itR = split(r);
        auto itL = split(l);

        auto it = itL;
        while (it != itR) {
            if (it->second.black) {
                --blackSegments;
                blackLength -= static_cast<long long>(it->second.r - it->first);
            }
            it = segments.erase(it);
        }

        if (l < r) {
            auto inserted = segments.emplace(l, Segment{r, makeBlack}).first;
            if (makeBlack) {
                ++blackSegments;
                blackLength += static_cast<long long>(r - l);
            }
            mergeAround(inserted);
        }

        std::cout << blackSegments << ' ' << blackLength << '\n';
    }

    return 0;
}
