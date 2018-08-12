class Positional {
    public:
    Positional(bool a = false, bool b = false, bool c = false, bool d = false,
               bool e = false, bool f = false, bool g = false, bool h = false);

    operator bool() const { return a_; }

    private:
    const bool a_;
    const bool b_;
    const bool c_;
    const bool d_;
    const bool e_;
    const bool f_;
    const bool g_;
    const bool h_;
};

Positional::Positional(bool a, bool b, bool c, bool d,
                       bool e, bool f, bool g, bool h)
        : a_(a), b_(b), c_(c), d_(d), e_(e), f_(f), g_(g), h_(h) {}

class Named {
    public:
    class Options {
        public:
        Options() : a_(false), b_(false), c_(false), d_(false),
                    e_(false), f_(false), g_(false), h_(false) {}
        Options& SetA(bool a) { a_ = a; return *this; }
        Options& SetB(bool b) { b_ = b; return *this; }
        Options& SetC(bool c) { c_ = c; return *this; }
        Options& SetD(bool d) { d_ = d; return *this; }
        Options& SetE(bool e) { e_ = e; return *this; }
        Options& SetF(bool f) { f_ = f; return *this; }
        Options& SetG(bool g) { g_ = g; return *this; }
        Options& SetH(bool h) { h_ = h; return *this; }

        private:
        friend class Named;
        bool a_;
        bool b_;
        bool c_;
        bool d_;
        bool e_;
        bool f_;
        bool g_;
        bool h_;
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
        Options() : a(false), b(false), c(false), d(false),
                    e(false), f(false), g(false), h(false) {}
        bool a;
        bool b;
        bool c;
        bool d;
        bool e;
        bool f;
        bool g;
        bool h;
    };

    Aggregate(const Options& options);

    operator bool() const { return options_.a; }

    private:
    const Options options_;
};

Aggregate::Aggregate(const Aggregate::Options& options) : options_(options) {}
