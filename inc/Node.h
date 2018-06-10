#ifndef __NODE_H__
#define __NODE_H__

#include "Element.h"
#include "ControllerIn.h"
#include "ControllerOut.h"

class Node {

    friend class Factory;

    Element* e;
    ControllerIn* in;
    ControllerOut* out;

    public:

    Node();

    Node(Element* e, ControllerIn* in, ControllerOut* out);

    string getId();

    ControllerIn* getControllerIn();
    ControllerOut* getControllerOut();

    void step();
    void commit();
    stringstream report();
};

#endif
