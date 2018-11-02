// 05 returning a pair
#include <mutex>
#include <queue>
#include <utility>
#include <iostream>
using std::cout;
using std::endl;

template <typename T> class locking_queue {
    using mutex = std::mutex;
    using lock_guard = std::lock_guard<mutex>;
    public:
    using value_type = typename std::queue<T>::value_type;
    using reference = typename std::queue<T>::reference;
    void push(const value_type& value) {
        lock_guard l(m_);
        q_.push(value);
    }
    void push(value_type&& value) {
        lock_guard l(m_);
        q_.push(value);
    }
    std::pair<value_type, bool> pop() {
        lock_guard l(m_);
        if (q_.empty()) return { value_type(), false };
        value_type value = std::move(q_.front());
        q_.pop();
        return { value, true };
    }

    private:
    std::queue<T> q_;
    mutex m_;
};

int main() {
    locking_queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    auto x = q.pop(); cout << x.second << " " << x.first << endl;
    x = q.pop(); cout << x.second << " " << x.first << endl;
    x = q.pop(); cout << x.second << " " << x.first << endl;
    x = q.pop(); cout << x.second << " " << x.first << endl;
}
