class Positional {
    public:
    Positional(bool a, bool b, bool c, bool d);

    operator bool() const { return a_; }

    private:
    const bool a_;
    const bool b_;
    const bool c_;
    const bool d_;
};

Positional::Positional(bool a, bool b, bool c, bool d)
        : a_(a), b_(b), c_(c), d_(d) {}


class Named {
    public:
    class Options {
        public:
        Options() : a_(false), b_(false), c_(false), d_(false) {}
        Options& SetA(bool a) { a_ = a; return *this; }
        Options& SetB(bool b) { b_ = b; return *this; }
        Options& SetC(bool c) { c_ = c; return *this; }
        Options& SetD(bool d) { d_ = d; return *this; }

        private:
        friend class Named;
        bool a_;
        bool b_;
        bool c_;
        bool d_;
    };

    Named(Options options);

    operator bool() const { return options_.a_; }

    private:
    const Options options_;
};

Named::Named(Named::Options options) : options_(options) {}

class Aggregate {
    public:
    struct Options {
        Options() : a(false), b(false), c(false), d(false) {}
        bool a;
        bool b;
        bool c;
        bool d;
    };

    Aggregate(const Options& options);

    operator bool() const { return options_.a; }

    private:
    const Options options_;
};

Aggregate::Aggregate(const Aggregate::Options& options) : options_(options) {}
