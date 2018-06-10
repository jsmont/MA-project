#include "Graph.h"
#include "utils.h"

Graph::Graph(){
    wires = vector<Wire*>(0);
    nodes = vector<Node*>(0);
}

void Graph::addNode(Node* n){
    for(int i = 0; i < nodes.size(); ++i){
        assert(nodes[i]->getId() != n->getId());
    }

    nodes.push_back(n);
}

void Graph::addWire(Wire* w){

    bool srcFound=false;
    bool destFound=false;

    assert(w->getSrcId() != w->getDestId());

    wires.push_back(w);

    for(int i = 0; i < nodes.size() && (!srcFound || !destFound); ++i){
        if(!srcFound && (w->getSrcId() == nodes[i]->getId())){
            srcFound = true;
            ControllerIn* in = nodes[i]->getControllerIn();
            in->addWire(w);
        }
        if(!destFound && (w->getDestId() == nodes[i]->getId())){
            destFound = true;
            ControllerOut* out = nodes[i]->getControllerOut();
            out->addWire(w);
        }
    }

    assert(srcFound);
    assert(destFound);

}

void Graph::step(){
    for(int i = 0; i < nodes.size(); ++i)
        nodes[i]->step();

    for(int i = 0; i < nodes.size(); ++i)
        nodes[i]->commit();

    for(int i = 0; i < wires.size(); ++i)
        wires[i]->commit();
}

stringstream Graph::report(){

    stringstream r;

    for(int i = 0; i < nodes.size(); ++i){
        r << nodes[i]->report().str();
    }

    for(int i = 0; i < wires.size(); ++i){
        r << wires[i]->report().str();
    }

    return r;
}
