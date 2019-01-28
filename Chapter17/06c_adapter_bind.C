// 06 using std::bind
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>
using std::cout;
using std::endl;
using namespace std::placeholders; // For _1, _2 etc

// std::bind example
int f3(int i, int j, int k) { return i + j + k; }
auto f2 = std::bind(f3, _1, _2, 42);

struct much_less {
    template <typename T> bool operator()(T x, T y) { return x < y && std::abs(x - y) > 0.2*std::max(std::abs(x), std::abs(y)); }
};

template<typename RandomIt> void sort_much_less(RandomIt first, RandomIt last) {
    auto f = std::bind(std::sort<RandomIt, much_less>, _1, _2, much_less());
    f(first, last, much_less());
}

int main() {
    cout << f2(2, 6) << endl;

    const size_t N = 20;
    std::vector<double> v(N);
    for (size_t i = 0; i < N; ++i) {
        v[i] = (N - i)*0.1;
    }
    sort_much_less(v.begin(), v.end());
    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
}
