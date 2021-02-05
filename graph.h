#include <atomic>
#include <iostream>
#include "HNode.h"
#include "utils.h"

using namespace std;

class ConcGraph {
private:
    HashTable<int, HashTable<int, int>*> *adjlist;
public:
    ConcGraph() {
        adjlist = new HashTable<int, HashTable<int, int>*>();
    }

    bool addV(int key) {
        // Check whether new HashTable is needed or nullptr is fine.
        return adjlist->insert(key, new HashTable<int, int>());
    }

    bool removeV(int key) {
        return adjlist->remove(key, nullptr);
    }

    bool containsV(int key) {
        return adjlist->contains(key);
    }

    bool addE(int key1, int key2) {
        int DUMMY_VAL = 0;
        // If either vertex is not present.
        if(!adjlist->contains(key1) || !adjlist->contains(key2))
            return false;
        
        // Inserts edge if not present already.
        return adjlist->at(key1)->insert(key2, DUMMY_VAL);
    }

    bool removeE(int key1, int key2) {
        int DUMMY_VAL = 0;
        // If key1 is not present then edge is already deleted.
        // If key2 is not present then check if we need to physically remove key2 from edgelist.
        if(!adjlist->contains(key1) || !adjlist->contains(key2))
            return false;

        // Removes edge if present.
        return adjlist->at(key1)->remove(key2, DUMMY_VAL);
    }

    bool containsE(int key1, int key2) {
        if(!adjlist->contains(key1) || !adjlist->contains(key2))
            return false;

        return adjlist->at(key1)->contains(key2);
    }

};
