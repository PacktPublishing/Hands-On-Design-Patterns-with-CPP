#include <vector>
#include <iostream>

class Building;
typedef Building* (*BuildingFactory)();

int building_type_count = 0;

std::vector<std::pair<int, BuildingFactory>> building_registry;
void RegisterBuilding(BuildingFactory factory) {
    building_registry.push_back(std::make_pair(building_type_count++, factory));
}

class Unit {};
class Knight : public Unit {
    public:
    Knight() { std::cout << "new Knight" << std::endl; }
};
class Mage : public Unit {
    public:
    Mage() { std::cout << "new Mage" << std::endl; }
};
class Spider : public Unit {
    public:
    Spider() { std::cout << "new Spider" << std::endl; }
};

class Building {
    public:
    static Building* MakeBuilding(int building_type) {
        BuildingFactory factory = building_registry[building_type].second;
        return factory();
    }
    virtual Unit* MakeUnit() const = 0;
};

class Castle : public Building {
    public:
    Castle() { std::cout << "new Castle" << std::endl; }
    static Building* MakeBuilding() { return new Castle; }
    static void Register() {
        ::RegisterBuilding(Castle::MakeBuilding);
    }
    Knight* MakeUnit() const { return new Knight; }
};

class Tower : public Building {
    public:
    Tower() { std::cout << "new Tower" << std::endl; }
    static Building* MakeBuilding() { return new Tower; }
    static void Register() {
        ::RegisterBuilding(Tower::MakeBuilding);
    }
    Mage* MakeUnit() const { return new Mage; }
};

class Mound : public Building {
    public:
    Mound() { std::cout << "new Mound" << std::endl; }
    static Building* MakeBuilding() { return new Mound; }
    static void Register() {
        ::RegisterBuilding(Mound::MakeBuilding);
    }
    Spider* MakeUnit() const { return new Spider; }
};

int main() {
    Castle::Register();
    Tower::Register();
    Mound::Register();
    Building* b0 = Building::MakeBuilding(0); 
    Building* b1 = Building::MakeBuilding(1);
    Building* b2 = Building::MakeBuilding(2);
    Unit* u0 = b0->MakeUnit();
    Unit* u1 = b1->MakeUnit();
    Unit* u2 = b2->MakeUnit();
}

