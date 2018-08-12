#include <iostream>

using std::cout;
using std::endl;

class SortedCollection;
class Collection {
    public:
    Collection() {}
    Collection filter();
    SortedCollection sort();
};

class SortedCollection : public Collection {
    public:
    SortedCollection() {}
    SortedCollection(const Collection&) {}
    SortedCollection search() { cout << "SortedCollection::search" << endl; return *this; }
    SortedCollection median() { cout << "SortedCollection::median" << endl; return *this; }
};

Collection Collection::filter() { cout << "Collection::filter" << endl; return *this; }
SortedCollection Collection::sort() { cout << "Collection::sort" << endl; return SortedCollection(*this); }

int main() {
    Collection c;
    c.sort().search().filter().median();
}
