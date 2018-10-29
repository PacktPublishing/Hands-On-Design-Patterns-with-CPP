// Date adapters
#include <ctime>
#include <iostream>
using std::cout;
using std::endl;

class USA_Date {
    public:
    explicit USA_Date(std::time_t t) : t_(t) {}
    friend std::ostream& operator<<(std::ostream& out, const USA_Date& d) {
        tm local_tm = *localtime(&d.t_);
        out << local_tm.tm_mon + 1 << "/" << local_tm.tm_mday << "/" << local_tm.tm_year + 1900;
        return out;
    }

    private:
    const std::time_t t_;
};

class European_Date {
    public:
    explicit European_Date(std::time_t t) : t_(t) {}
    friend std::ostream& operator<<(std::ostream& out, const European_Date& d) {
        tm local_tm = *localtime(&d.t_);
        out << local_tm.tm_mday << "/" << local_tm.tm_mon + 1 << "/" << local_tm.tm_year + 1900;
        return out;
    }

    private:
    const std::time_t t_;
};

class ISO_Date {
    public:
    explicit ISO_Date(std::time_t t) : t_(t) {}
    friend std::ostream& operator<<(std::ostream& out, const ISO_Date& d) {
        tm local_tm = *localtime(&d.t_);
        out << local_tm.tm_year + 1900 << "/" << local_tm.tm_mon + 1 << "/" << local_tm.tm_mday;
        return out;
    }

    private:
    const std::time_t t_;
};

int main() {
    const std::time_t now = std::time(NULL);
    cout << "Today in USA: " << USA_Date(now) << ", Europe: " << European_Date(now) << ", ISO: " << ISO_Date(now) << endl;
}
