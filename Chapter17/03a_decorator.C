// 03 with dynamic memory allocation
#include <iostream>
#include <memory>
using std::cout;
using std::endl;

// Basic decorator
class Unit {
    public:
    Unit(double strength, double armor) : strength_(strength), armor_(armor) {}
    virtual bool hit(Unit& target) { return attack() > target.defense(); }
    virtual double attack() = 0;
    virtual double defense() = 0;
    protected:
    double strength_;
    double armor_;
};
using Unit_ptr = std::unique_ptr<Unit>;

class Knight : public Unit {
    public:
    Knight(double strength, double armor) : Unit(strength, armor), charge_bonus_(0) {}
    double attack() { double res = strength_ + sword_bonus_ + charge_bonus_; charge_bonus_ = 0; return res; }
    double defense() { return armor_ + plate_bonus_; }
    void charge() { charge_bonus_ = 1; }
    protected:
    double charge_bonus_;
    static constexpr double sword_bonus_ = 2;
    static constexpr double plate_bonus_ = 3;
};
using Knight_ptr = std::unique_ptr<Knight>;

class Ogre : public Unit {
    public:
    using Unit::Unit;
    double attack() { return strength_ + club_penalty_; }
    double defense() { return armor_ + leather_penalty_; }
    protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

class Troll : public Unit {
    public:
    using Unit::Unit;
    double attack() { return strength_ + mace_bonus_; }
    double defense() { return armor_ + hide_bonus_; }
    protected:
    static constexpr double mace_bonus_ = 3;
    static constexpr double hide_bonus_ = 8;
};

template <typename To, typename From> static std::unique_ptr<To> move_cast(std::unique_ptr<From>& p) {
#ifndef NDEBUG
    auto p1 = std::unique_ptr<To>(dynamic_cast<To*>(p.release()));
    assert(p1);
    return p1;
#else 
    return std::unique_ptr<To>(static_cast<To*>(p.release()));
#endif
}

template <typename U> class VeteranUnit : public U {
    public:
    template <typename P> VeteranUnit(P&& p, double strength_bonus, double armor_bonus) : U(std::move(*move_cast<U>(p))), strength_bonus_(strength_bonus), armor_bonus_(armor_bonus) {}
    double attack() { return U::attack() + strength_bonus_; }
    double defense() { return U::defense() + armor_bonus_; }
    private:
    double strength_bonus_;
    double armor_bonus_;
};

template <typename U> class DebugDecorator : public U {
    public:
    using U::U;
    template <typename P> DebugDecorator(P&& p) : U(std::move(*move_cast<U>(p))) {}
    double attack() { double res = U::attack(); cout << "Attack: " << res << endl; return res; }
    double defense() { double res = U::defense(); cout << "Defense: " << res << endl; return res; }
};

int main() {
    Knight_ptr k(new DebugDecorator<Knight>(10, 5));
    Unit_ptr o(new DebugDecorator<Ogre>(12, 2));
    k->charge();
    cout << "Knight hits Ogre: " << k->hit(*o) << endl;
    Unit_ptr t(new Troll(14, 10));
    cout << "Knight hits Troll: " << k->hit(*t) << endl;
    Knight_ptr vk(new DebugDecorator<VeteranUnit<Knight>>(k, 7, 2));
    cout << "Veteran Knight hits Troll: " << vk->hit(*t) << endl;
    Unit_ptr vo(new DebugDecorator<VeteranUnit<Ogre>>(o, 1, 9));
    cout << "Veteran Knight hits Veteran Ogre: " << vk->hit(*vo) << endl;
    Unit_ptr vvo(new DebugDecorator<VeteranUnit<VeteranUnit<Ogre>>>(vo, 1, 9));
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
    vk->charge();
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
}
