// 01 with non-wrapped function
#include <iostream>
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

class Ogre : public Unit {
    public:
    using Unit::Unit;
    double attack() { return strength_ + club_penalty_; }
    double defense() { return armor_ + leather_penalty_; }
    void Beserk() { strength_ += 5; armor_ -= 2; }
    protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

class VeteranUnit : public Unit {
    public:
    VeteranUnit(Unit& unit, double strength_bonus, double armor_bonus) : Unit(strength_bonus, armor_bonus), unit_(unit) {}
    double attack() { return unit_.attack() + strength_; }
    double defense() { return unit_.defense() + armor_; }
    private:
    Unit& unit_;
};

int main() {
    Knight k(10, 5);
    Ogre o(12, 2);
    k.charge();
    cout << "Knight hits Ogre: " << k.hit(o) << endl;
    VeteranUnit vk(k, 7, 2);
    //vk.charge(); // Does not compile
    cout << "Veteran Knight hits Ogre: " << vk.hit(o) << endl;
}
