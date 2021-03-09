#include <atomic>
#include <iostream>
#include "utils.h"
using namespace std;

template <typename T, typename S>
struct Node {
    T key;
    S value;
    atomic<Node *> next; 

    Node(T k) {
        key = k;
        value = nullptr;
        next.store(nullptr, memory_order_seq_cst);
    }
    Node(T k, S val) {
        key = k;
        value = val;
        next.store(nullptr, memory_order_seq_cst);
    }
};

template <typename T, typename S>
class List {
public:
    Node<T,S> *Head, *Tail;

    List() {
        Head = new Node<T,S>(INT_MIN);
        Head->next.store(NULL,memory_order_seq_cst);
        Head->value = nullptr;
        //Head ->marked.store(false,memory_order_seq_cst);
        Tail = new Node<T,S>(INT_MAX);
        Tail->next.store(NULL,memory_order_seq_cst);
        Tail->value = nullptr;
        //Tail ->marked.store(false,memory_order_seq_cst);
        Head->next.store(Tail,memory_order_seq_cst);
    }

    bool insert(T key, S value) {
        Node<T,S> *newNode = Node<T,S>(key,value);
        Node<T,S> *pred, *curr;
        while(true) {
            locate(&pred, &curr, key);
            if(curr->key == key){
                return false;
            }
            else{
                newNode->next.store(curr,memory_order_seq_cst);  
                if(pred->next.compare_exchange_strong(curr, newNode, memory_order_seq_cst)) 
                    return true;
            }
        }
    }

    bool remove(T key) {
        Node<T,S> *pred, *curr, *succ;
        pred = Head;
        while(true) {
		    locate(&pred, &curr, key);
		    if(curr->key != key)
			    return false;
		    succ = curr->next.load(memory_order_seq_cst);
		    if(!is_marked_ref((long) succ))
			    if(atomic_compare_exchange_strong_explicit(&curr->next, &succ,(Node<T,S>*)get_marked_ref((long)succ), memory_order_seq_cst, memory_order_seq_cst))
				    break;
	    }
	    if(!atomic_compare_exchange_strong_explicit(&pred->next, &curr, succ, memory_order_seq_cst, memory_order_seq_cst))
		    locate(&pred, &curr, curr->key);
	    return true;
    }

    bool contains(T key, Node<T,S> **n) {
        Node<T,S> *curr, *pred;
        pred = Head;
	    locate(&pred, &curr, key);
	    if((!curr->next.load(memory_order_seq_cst)) || curr->key != key)
		    return false;
	    else {
	        *n = curr; 
	        return true;
		} 
    }

    void locate(Node<T,S> **n1, Node<T,S> **n2, int key) {
        Node<T,S> *succ, *curr, *pred;
        retry1:
	    while(true) {
            Node<T,S> *t = Head;
            Node<T,S> *t_next = t->next.load(memory_order_seq_cst);
            /* Find pred and curr */
            do {
                if(!is_marked_ref((long) t_next)) {
                    pred = t;
                    succ = t_next;
                }
                t = (Node<T,S> *) get_unmarked_ref((long)t_next);
                if(!t->next.load()) break;
                t_next = t->next.load(memory_order_seq_cst);
            } while(is_marked_ref((long) t_next) || (t->key < key));
            
            curr = t;
            /* Check that nodes are adjacent */
            if(succ == curr) {
                if(curr->next.load(memory_order_seq_cst) && is_marked_ref((long)curr->next.load(memory_order_seq_cst)))
                    goto retry1;
                else {
                    (*n1) = pred;
                    (*n2) = curr;
                    return ;
                }
            }
            /* Remove one or more marked nodes */
            if(atomic_compare_exchange_strong_explicit(& pred->next, &succ, curr,  memory_order_seq_cst, memory_order_seq_cst)) {
                if (curr->next.load(memory_order_seq_cst) && is_marked_ref((long) curr->next.load(memory_order_seq_cst)))
                    goto retry1;
                else {
                    (*n1) = pred;
                    (*n2) = curr;
                    return ;
                }
            }
	    }
    }
};