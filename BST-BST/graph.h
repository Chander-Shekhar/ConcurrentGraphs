#include <atomic>
#include <iostream>
#include "BST.h"
// #include "utils.h"
#include <bits/stdc++.h>

using namespace std;

class ConcGraph {
private:
    BST *graph;
public:
    ConcGraph() {
        graph = new BST();
    }
    
    bool addV(int key) {
        return graph->add(key);
    }

    bool removeV(int key) {
        return graph->remove(key);
    }

    bool containsV(int key) {
        return graph->contains(key);
    }

    bool addE(int key1, int key2) {
        BST::Node* pred1, * curr1;
        BST::Operation* predOp1, * currOp1;
        BST::Node* pred2, * curr2;
        BST::Operation* predOp2, * currOp2;
        
        // Locate both the vertices 
        auto p1=graph->find(key1, pred1, predOp1, curr1, currOp1, &(graph->root));
        auto p2=graph->find(key2, pred2, predOp2, curr2, currOp2, &(graph->root));

        // If either vertex is not present, abort.
        if(p1!=FOUND || p2!=FOUND)
            return false;
        
        // Inserts edge if not present already.
        return (curr1->rootEdge->add(key2,curr2));
    }

    bool removeE(int key1, int key2) {
        BST::Node* pred1, * curr1;
        BST::Operation* predOp1, * currOp1;
        BST::Node* pred2, * curr2;
        BST::Operation* predOp2, * currOp2;
        
        // Locate both the vertices 
        auto p1=graph->find(key1, pred1, predOp1, curr1, currOp1, &(graph->root));
        auto p2=graph->find(key2, pred2, predOp2, curr2, currOp2, &(graph->root));
        
        // If either vertex is not present, abort.
        if(p1!=FOUND || p2!=FOUND)
            return false;
        // Removes edge if present.
        return (curr1->rootEdge->removeE(key2));
    }

    bool containsE(int key1, int key2) {
        BST::Node* pred1, * curr1;
        BST::Operation* predOp1, * currOp1;
        BST::Node* pred2, * curr2;
        BST::Operation* predOp2, * currOp2;
        // Locate both the vertices
        auto p1=graph->find(key1, pred1, predOp1, curr1, currOp1, &(graph->root));
        auto p2=graph->find(key2, pred2, predOp2, curr2, currOp2, &(graph->root));
        
        // If either vertex is not present, abort.
        if(p1!=FOUND || p2!=FOUND)
            return false;
            
        return curr1->rootEdge->contains(key2) && GETFLAG(curr1->backNode)!=MARK && GETFLAG(curr2->backNode)!=MARK;
    }

    void initGraph(int n){
        for(int i=0;i<n;i++)
            addV(i);
    }
};
