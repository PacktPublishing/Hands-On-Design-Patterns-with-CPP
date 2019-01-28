// 06 with container adapter
#include <algorithm>
#include <vector>
#include <iostream>
using std::cout;
using std::endl;

struct much_less {
    template <typename T> bool operator()(T x, T y) { return x < y && std::abs(x - y) > tolerance*std::max(std::abs(x), std::abs(y)); }
    static constexpr double tolerance = 0.2;
};

template<typename Container> void sort_much_less(Container& c) {
    std::sort(c.begin(), c.end(), much_less());
}

int main() {
    const size_t N = 20;
    std::vector<double> v(N);
    for (size_t i = 0; i < N; ++i) {
        v[i] = (N - i)*0.1;
    }
    sort_much_less(v);
    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
}
