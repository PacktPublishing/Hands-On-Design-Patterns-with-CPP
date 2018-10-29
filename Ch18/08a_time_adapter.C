// 08 refactored
#include <ctime>
#include <iostream>
using std::cout;
using std::endl;

template <size_t F> class Date {
    public:
    explicit Date(std::time_t t) : t_(t) {}
    friend std::ostream& operator<<(std::ostream& out, const Date& d) {
        tm local_tm = *localtime(&d.t_);
        const int t[3] = { local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year + 1900 };
        constexpr size_t i1 = F/100;
        constexpr size_t i2 = (F - i1*100)/10;
        constexpr size_t i3 = F - i1*100 - i2*10;
        out << t[i1] << "/" << t[i2] << "/" << t[i3];
        return out;
    }

    private:
    const std::time_t t_;
};

using USA_Date = Date<102>;
using European_Date = Date<12>;
using ISO_Date = Date<210>;

int main() {
    const std::time_t now = std::time(NULL);
    cout << "Today in USA: " << USA_Date(now) << ", Europe: " << European_Date(now) << ", ISO: " << ISO_Date(now) << endl;
}
