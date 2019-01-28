#include <functional>
#include <iostream>

enum Outcome { SUCCESS, FAIL_RETURN, FAIL_THROW };

// Demo disk storage, does nothing useful but may throw exception.
class Storage {
    public:
    Storage() : i_(0) {}
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i1_ = i_; i_ = i;
        return true;
    }
    void undo() {
        i_ = i1_;
    }
    int get() const { return i_; }
    private:
    int i_;
    int i1_;
};

// Demo memory index, does nothing useful but may throw exception.
class Index {
    public:
    Index() : i_(0) {}
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i1_ = i_; i_ = i;
        return true;
    }
    void undo() {
        i_ = i1_;
    }
    int get() const { return i_; }
    private:
    int i_;
    int i1_;
};

class ScopeGuard {
    public:
    template <typename Func> ScopeGuard(Func&& func) : commit_(false), func_(func) {}
    template <typename Func> ScopeGuard(const Func& func) : commit_(false), func_(func) {}
    ~ScopeGuard() { if (!commit_) func_(); }
    void commit() const noexcept { commit_ = true; }
    ScopeGuard(ScopeGuard&& other) : commit_(other.commit_), func_(other.func_) { other.commit(); }
    private:
    mutable bool commit_;
    std::function<void()> func_;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

int main() {
    Storage S;
    Index I;
    try {
        S.insert(42, SUCCESS);
        ScopeGuard SG([&] { S.undo(); });
        I.insert(42, FAIL_THROW);
        SG.commit();
    } catch (...) {
    }

    if (S.get() != I.get()) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
    else std::cout << "Database OK" << std::endl;
}
