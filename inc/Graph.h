#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>

#include "Node.h"
#include "Wire.h"

class Graph {
    vector<Wire*> wires;
    vector<Node*> nodes;

    public:
    Graph();
    
    void addNode(Node* n);
    void addWire(Wire* w);

    void step();
    stringstream report();
};

#endif
