#include <iostream>

using std::cout;
using std::endl;

int main() {
    int i, j;
    auto& out1 = cout.operator<<(i);
    out1.operator<<(j);
}
