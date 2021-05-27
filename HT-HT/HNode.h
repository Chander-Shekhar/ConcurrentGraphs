#include <iostream>
#include <unordered_map>
#include <atomic>
#include "FSet.h"
#include <bits/stdc++.h>
using namespace std;

template<typename T,typename S>
class HNode{
public:
    atomic<FSet<T,S>*> *buckets;
    int size;
    // int used;
    HNode<T,S> *pred;
    HNode(int capacity, HNode<T,S> *pred) {
        buckets = new atomic<FSet<T,S>*>[capacity];
        for(int i=0;i<capacity;i++)
            buckets[i].store(nullptr);
        size = capacity;
        this->pred = pred;
    }
};

template<typename T,typename S>
class HashTable {
private:
    atomic<HNode<T,S>*> head;

    bool apply(OPType type, T key, S value) {
        FSetOP<T,S> *op = new FSetOP<T,S>(type, key, value);
        while(true) {
            HNode<T,S> *t = head.load(memory_order_seq_cst);
            FSet<T,S> *curr_bucket = t->buckets[key % t->size].load(memory_order_seq_cst);
            if(!curr_bucket) {
                curr_bucket = initBucket(t, (key % t->size));

                // The resize heuristic can be changed according to preference.
                // This has no effect on the correctness of the algorithm.
                if(type == INS){
                    if(curr_bucket->getHead()->map->size()>=20){
                        resize(true);
                    }
                }
            }
            if(curr_bucket->invoke(op))
                return op->getResponse();
        }
    }

    bool apply(OPType type, T key, S value, typename unordered_map<T, HashTable<T, S>*>::iterator u, typename unordered_map<T, HashTable<T, S>*>::iterator v) {
        FSetOP<T,S> *op = new FSetOP<T,S>(type, key, value);
        while(true) {
            HNode<T,S> *t = head.load(memory_order_seq_cst);
            FSet<T,S> *curr_bucket = t->buckets[key % t->size].load(memory_order_seq_cst);
            if(!curr_bucket) {
                curr_bucket = initBucket(t, (key % t->size));
                
                // The resize heuristic can be changed according to preference.
                // This has no effect on the correctness of the algorithm.
                if(type == INS){
                    if(curr_bucket->getHead()->map->size()>=20){
                        resize(true);
                    }
                }
            }
            if(curr_bucket->edgeInvoke(op, (int **)(&(u->second)), (int **)(&(v->second))))
                return op->getResponse();
        }
    }

    void resize(bool grow) {
        HNode<T,S> *t = head.load(memory_order_seq_cst);
        if(t->size <= 1 and !grow)
            return ;
        for(int i=0;i<t->size;i++)
            initBucket(t, i);
        t->pred = nullptr;
        int size = grow ? t->size*2 :t->size/2;
        // atomic<FSet<T,S>*> *buckets = new atomic<FSet<T,S>*>[size];
        HNode<T,S> *t_dash = new HNode<T,S>(size, t);
        // Confused if it should be in a loop
        head.compare_exchange_strong(t, t_dash);
    }

    FSet<T,S> *initBucket(HNode<T,S> *t, int i) {
        FSet<T,S> *b = t->buckets[i].load(memory_order_seq_cst);
        FSet<T,S> *m;
        HNode<T,S> *s = t->pred;
        if(!b and s) {
            unordered_map<T,S> *new_set = new unordered_map<T,S>();
            if(t->size == s->size*2) {
                m = s->buckets[i % s->size].load(memory_order_seq_cst);
                unordered_map<T,S> set_1 = *m->freeze();
                for(auto itr : set_1) {
                    if(itr.first % t->size == i)
                        new_set->insert(itr);
                }
            }
            else {
                m = s->buckets[i];
                FSet<T,S> *n = s->buckets[i+t->size];
                unordered_map<T,S> set_1 = *m->freeze(), set_2 = *n->freeze(); 
                for(auto itr : set_1)
                    new_set->insert(itr);
                for(auto itr : set_2)
                    new_set->insert(itr);
            }
            FSet<T,S> *b_dash = new FSet<T,S>(new_set, true);
            // Confused if it should be in a loop
            FSet<T,S> *nil = nullptr;
            t->buckets[i].compare_exchange_strong(nil, b_dash);
        }
        return t->buckets[i].load(memory_order_seq_cst);
    }
public:
    HashTable() {
        head.store(new HNode<T,S>(1, nullptr));
        head.load(memory_order_seq_cst)->buckets[0].store(new FSet<T,S>(new unordered_map<T,S>(), true));
    }

    bool insert(T key, S value) {
        bool resp = apply(INS, key, value);
        return resp;
    }

    bool insert(T key, S value, typename unordered_map<T, HashTable<T, S>*>::iterator u, typename unordered_map<T, HashTable<T, S>*>::iterator v) {
        bool resp = apply(INS, key, value, u, v);
        return resp;
    }

    bool remove(T key, S value) {
        bool resp = apply(REM, key, value);
        HNode<T,S> *t = head.load(memory_order_seq_cst);
        int size=t->size;
        // The resize heuristic can be changed according to preference.
        // This has no effect on the correctness of the algorithm.
        if(size >= 3){
            int a=rand()%size;
            int b=(rand()%size+a)%size;
            
            if(t->buckets[a].load(memory_order_seq_cst)->getHead()->map->size() <=5 && t->buckets[b].load(memory_order_seq_cst)->getHead()->map->size()<=5)
                resize(false);
        }
        return resp;
    }

    bool remove(T key, S value, typename unordered_map<T, HashTable<T, S>*>::iterator u, typename unordered_map<T, HashTable<T, S>*>::iterator v) {
        bool resp = apply(REM, key, value, u, v);
        HNode<T,S> *t = head.load(memory_order_seq_cst);
        int size=t->size;
        // The resize heuristic can be changed according to preference.
        // This has no effect on the correctness of the algorithm.
        if(size >= 3){
            int a=rand()%size;
            int b=(rand()%size+a)%size;
            
            if(t->buckets[a].load(memory_order_seq_cst)->getHead()->map->size() <=5 && t->buckets[b].load(memory_order_seq_cst)->getHead()->map->size()<=5)
                resize(false);
        }
        return resp;
    }

    bool contains(T key) {
        HNode<T,S> *t = head.load(memory_order_seq_cst);
        FSet<T,S> *curr_bucket = t->buckets[key % t->size].load(memory_order_seq_cst);
        if(!curr_bucket) {
            HNode<T,S> *prev_node = t->pred;
            if(prev_node)
                curr_bucket = prev_node->buckets[key % prev_node->size].load(memory_order_seq_cst);
            else
                curr_bucket = t->buckets[key % t->size].load(memory_order_seq_cst);
        }
        return curr_bucket->hasMember(key);
    }
    pair<typename unordered_map<T,S>::iterator, typename unordered_map<T,S>::iterator> at(T key) {
        HNode<T,S> *t = head.load(memory_order_seq_cst);
        FSet<T,S> *curr_bucket = t->buckets[key % t->size].load(memory_order_seq_cst);
        if(!curr_bucket) {
            HNode<T,S> *prev_node = t->pred;
            if(prev_node)
                curr_bucket = prev_node->buckets[key % prev_node->size].load(memory_order_seq_cst);
            else
                curr_bucket = t->buckets[key % t->size].load(memory_order_seq_cst);
        }
        return curr_bucket->at(key);
    }
};
