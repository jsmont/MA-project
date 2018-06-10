#include "Node.h"

Node::Node(){
    this->e = NULL;
    this->in = NULL;
    this->out = NULL;
}

Node::Node(Element* e, ControllerIn* in, ControllerOut* out){
    this->e = e;
    this->in = in;
    this->out = out;
}

string Node::getId(){
    return e->getId();
}

ControllerIn* Node::getControllerIn(){
    return in;
}

ControllerOut* Node::getControllerOut(){
    return out;
}

void Node::step(){
    in->step();
    out->step();
}

void Node::commit(){
    e->commit();
}

stringstream Node::report(){
    return e->report();
}
