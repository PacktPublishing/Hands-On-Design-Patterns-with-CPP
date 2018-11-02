// Adapter for template template
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

template <template <typename> class Container, typename T>
std::ostream& operator<<(std::ostream& out, const Container<T>& c) {
    bool first = true;
    for (auto x : c) {
        if (!first) out << ", ";
        first = false;
        out << x;
    }
    return out;
}

template <typename T> class vector1 : public std::vector<T> {
    using base_t = std::vector<T>;
    public:
    using base_t::base_t;
    //using std::vector<T>::vector;     // This works too
};
//template <typename T> using vector1 = std::vector<T>; // Not deduced by template deduction!

template <typename T> class array1 : public std::array<T, 10> {
    using base_t = std::array<T, 10>;
    public:
    using base_t::base_t;
};

int main() {
    const size_t N = 10;
    Buffer<int> b(N);
    //std::vector<int> v(N);      // Does not compile - 2 argument template
    vector1<int> v(N);
    //std::array<int, N> a;       // Does not compile - 2 argument template
    array1<int> a;
    for (size_t i = 0; i < N; ++i) {
        a[i] = v[i] = b[i] = (N - i);
    }
    cout << b << endl;
    cout << v << endl;
    cout << a << endl;
}
