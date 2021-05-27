#include <atomic>
#include <iostream>
#include "HNode.h"
// #include "utils.h"
#include <bits/stdc++.h>

using namespace std;

class ConcGraph {
private:
    HashTable<int, HashTable<int, int**>*> *adjlist;
public:
    ConcGraph() {
        adjlist = new HashTable<int, HashTable<int, int**>*>();
    }
    
    bool addV(int key) {
        return adjlist->insert(key, new HashTable<int, int**>());
    }

    bool removeV(int key) {
        return adjlist->remove(key, nullptr);
    }

    bool containsV(int key) {
        return adjlist->contains(key);
    }

    bool addE(int key1, int key2) {
        // Locate both the vertices 
        pair<unordered_map<int, HashTable<int, int**>*>::iterator, unordered_map<int, HashTable<int, int**>*>::iterator> p1 = adjlist->at(key1);
        pair<unordered_map<int, HashTable<int, int**>*>::iterator, unordered_map<int, HashTable<int, int**>*>::iterator> p2 = adjlist->at(key2);
        // If either vertex is not present, abort.
        if(p1.first == p1.second || p2.first == p2.second)
            return false;
        // Inserts edge if not present already.
        // Pass the pointer to check if any of the vertex is marked.
        return (p1.first->second->insert(key2, (int **)(&(p2.first->second)), p1.first, p2.first));
    }

    bool removeE(int key1, int key2) {
        // Locate both the vertices 
        pair<unordered_map<int, HashTable<int, int**>*>::iterator, unordered_map<int, HashTable<int, int**>*>::iterator> p1 = adjlist->at(key1);
        pair<unordered_map<int, HashTable<int, int**>*>::iterator, unordered_map<int, HashTable<int, int**>*>::iterator> p2 = adjlist->at(key2);
        // If either vertex is not present, abort.
        if(p1.first == p1.second || p2.first == p2.second)
            return false;
        // Removes edge if present.
        // Pass the pointer to check if any of the vertex is marked.
        return (p1.first->second)->remove(key2, (int **)nullptr, p1.first, p2.first);
    }

    bool containsE(int key1, int key2) {
        // Locate both the vertices 
        pair<unordered_map<int, HashTable<int, int**>*>::iterator, unordered_map<int, HashTable<int, int**>*>::iterator> p1 = adjlist->at(key1);
        pair<unordered_map<int, HashTable<int, int**>*>::iterator, unordered_map<int, HashTable<int, int**>*>::iterator> p2 = adjlist->at(key2);
        // If either vertex is not present, abort.
        if(p1.first == p1.second || p2.first == p2.second)
            return false;

        return (p1.first->second->contains(key2) && !is_marked_ref((long)p1.first->second) && !is_marked_ref((long)p2.first->second));
    }

    void initGraph(int n){
        for(int i=0;i<n;i++)
            addV(i);
    }
};
