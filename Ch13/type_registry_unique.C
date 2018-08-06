#include <vector>
#include <iostream>
#include <memory>

class Building;
typedef Building* (*BuildingFactory)();

int building_type_count = 0;

std::vector<std::pair<int, BuildingFactory>> building_registry;
void RegisterBuilding(BuildingFactory factory) {
    building_registry.push_back(std::make_pair(building_type_count++, factory));
}

class Building {
    public:
    static auto MakeBuilding(int building_type) {
        BuildingFactory factory = building_registry[building_type].second;
        return std::unique_ptr<Building>(factory());
    }
};

class Farm : public Building {
    public:
    Farm() { std::cout << "new Farm" << std::endl; }
    static Building* MakeBuilding() { return new Farm; }
    static void Register() {
        ::RegisterBuilding(Farm::MakeBuilding);
    }
};

class Forge : public Building {
    public:
    Forge() { std::cout << "new Forge" << std::endl; }
    static Building* MakeBuilding() { return new Forge; }
    static void Register() {
        ::RegisterBuilding(Forge::MakeBuilding);
    }
};

int main() {
    Farm::Register();
    Forge::Register();
    std::unique_ptr<Building> b0 = Building::MakeBuilding(0);
    std::unique_ptr<Building> b1 = Building::MakeBuilding(1);
}

