#include <atomic>
#include <iostream>
#include "HNode.h"
#include "utils.h"
#include <bits/stdc++.h>

using namespace std;

class ConcGraph {
private:
    HashTable<int, HashTable<int, int*>*> *adjlist;
public:
    ConcGraph() {
        adjlist = new HashTable<int, HashTable<int, int*>*>();
    }
    
    bool addV(int key) {
        return adjlist->insert(key, new HashTable<int, int*>());
    }

    bool removeV(int key) {
        return adjlist->remove(key, nullptr);
    }

    bool containsV(int key) {
        return adjlist->contains(key);
    }

    bool addE(int key1, int key2) {
        // u = locateV(key1);
        // v = locateV(key2);
        // if(!adjlist->contains(key1) || !adjlist->contains(key2))
        //     return false;
        // If either vertex is not present.
        pair<unordered_map<int, HashTable<int, int*>*>::iterator, unordered_map<int, HashTable<int, int*>*>::iterator> p1 = adjlist->at(key1);
        pair<unordered_map<int, HashTable<int, int*>*>::iterator, unordered_map<int, HashTable<int, int*>*>::iterator> p2 = adjlist->at(key2);
        if(p1.first == p1.second || p2.first == p2.second)
            return false;
        // Inserts edge if not present already.
        return (p1.first->second->insert(key2, (int *)(p2.first->second), p1.first, p2.first));
    }

    bool removeE(int key1, int key2) {
        // If key1 is not present then edge is already deleted.
        // If key2 is not present then check if we need to physically remove key2 from edgelist.
        pair<unordered_map<int, HashTable<int, int*>*>::iterator, unordered_map<int, HashTable<int, int*>*>::iterator> p1 = adjlist->at(key1);
        pair<unordered_map<int, HashTable<int, int*>*>::iterator, unordered_map<int, HashTable<int, int*>*>::iterator> p2 = adjlist->at(key2);
        if(p1.first == p1.second || p2.first == p2.second)
            return false;
        // Removes edge if present.
        return (p1.first->second)->remove(key2, (int *)nullptr, p1.first, p2.first);
    }

    bool containsE(int key1, int key2) {
        // if(!adjlist->contains(key1) || !adjlist->contains(key2))
        //     return false;
        pair<unordered_map<int, HashTable<int, int*>*>::iterator, unordered_map<int, HashTable<int, int*>*>::iterator> p1 = adjlist->at(key1);
        pair<unordered_map<int, HashTable<int, int*>*>::iterator, unordered_map<int, HashTable<int, int*>*>::iterator> p2 = adjlist->at(key2);
        if(p1.first == p1.second || p2.first == p2.second)
            return false;

        return (p1.first->second->contains(key2) && !is_marked_ref((long)p1.first->second) && !is_marked_ref((long)p2.first->second));
    }

};
