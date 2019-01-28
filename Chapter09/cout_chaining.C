#include <iostream>

using std::cout;
using std::endl;

int main() {
    int i = 0, j = 0;
    auto& out1 = cout.operator<<(i);
    out1.operator<<(j);
}
