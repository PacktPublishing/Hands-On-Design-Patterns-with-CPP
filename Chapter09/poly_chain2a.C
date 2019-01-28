#include <iostream>

using std::cout;
using std::endl;

class SortedCollection;
class Collection {
    public:
    Collection() {}
    virtual Collection filter();
    SortedCollection sort();
    virtual SortedCollection median();
};

class SortedCollection : public Collection {
    public:
    SortedCollection() {}
    SortedCollection(const Collection&) {}
    virtual SortedCollection filter() { cout << "SortedCollection::filter" << endl; return *this; }     // Not covariant return type, will not compile!
    SortedCollection search() { cout << "SortedCollection::search" << endl; return *this; }
    SortedCollection median() { cout << "SortedCollection::median" << endl; return *this; }
};

Collection Collection::filter() { cout << "Collection::filter" << endl; return *this; }
SortedCollection Collection::sort() { cout << "Collection::sort" << endl; return SortedCollection(*this); }
SortedCollection Collection::median() { cout << "Collection::median!!!" << endl; return SortedCollection(*this); }

int main() {
    Collection c;
    c.sort().search().filter().median();
}
