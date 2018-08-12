#include <iostream>

using std::cout;
using std::endl;

class City {
    public:
    enum center_t { KEEP, PALACE, CITADEL };
    City(size_t number_of_buildings,
            size_t number_of_towers,
            size_t guard_strength,
            center_t center = KEEP,
            bool with_forge = false,
            bool with_granar = false,
            bool has_fresh_wate = false,
            bool is_coasta = false,
            bool has_fores = false)
    {}
}; 

int main() {
    City Capital(2, 1, City::KEEP);
}
