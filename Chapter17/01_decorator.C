// Basic decorator
#include <iostream>
using std::cout;
using std::endl;

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
    using Unit::Unit;
    double attack() { return strength_ + sword_bonus_; }
    double defense() { return armor_ + plate_bonus_; }
    protected:
    static constexpr double sword_bonus_ = 2;
    static constexpr double plate_bonus_ = 3;
};

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
    cout << "Knight hits Ogre: " << k.hit(o) << endl;
    Troll t(14, 10);
    cout << "Knight hits Troll: " << k.hit(t) << endl;
    VeteranUnit vk(k, 7, 2);
    cout << "Veteran Knight hits Troll: " << vk.hit(t) << endl;
    VeteranUnit vo(o, 1, 9);
    cout << "Veteran Knight hits Veteran Ogre: " << vk.hit(vo) << endl;
    VeteranUnit vvo(vo, 1, 9);
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk.hit(vvo) << endl;
}
