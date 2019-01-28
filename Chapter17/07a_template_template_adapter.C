// Alternative to 07 using variadic template
#include <vector>
#include <array>
#include <iostream>
using std::cout;
using std::endl;

template <typename T> class Buffer {
    public:
    explicit Buffer(size_t N) : N_(N), buffer_(new T[N_]) {}
    ~Buffer() { delete [] buffer_; }
    size_t size() const { return N_; }
    T& operator[](size_t i) { return buffer_[i]; }
    const T& operator[](size_t i) const { return buffer_[i]; }
    T* begin() const { return buffer_; }
    T* end() const { return buffer_ + N_; }

    private:
    const size_t N_;
    T* const buffer_;
};

template <typename T, template <typename, typename ...> class Container, typename ... Args>
std::ostream& operator<<(std::ostream& out, const Container<T, Args ...>& c) {
    bool first = true;
    for (auto x : c) {
        if (!first) out << ", ";
        first = false;
        out << x;
    }
    return out;
}

template <typename T, template <typename, size_t> class Container, size_t N>
std::ostream& operator<<(std::ostream& out, const Container<T, N>& c) {
    bool first = true;
    for (auto x : c) {
        if (!first) out << ": ";
        first = false;
        out << x;
    }
    return out;
}

int main() {
    const size_t N = 10;
    Buffer<int> b(N);
    std::vector<int> v(N);
    std::array<int, N> a;
    for (size_t i = 0; i < N; ++i) {
        a[i] = v[i] = b[i] = (N - i);
    }
    cout << b << endl;
    cout << v << endl;
    cout << a << endl;
}
