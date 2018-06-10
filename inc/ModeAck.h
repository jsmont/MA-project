#ifndef __MODE_ACK_H__
#define __MODE_ACK_H__

#include "Element.h"
#include "Wire.h"
#include "ControllerIn.h"
#include "ControllerOut.h"
#include <vector>

enum class AckMessage{
    REQ,
    ACK
};

enum class ControllerInState {
    IDLE,
    WAITING
};

enum class ControllerOutState{
    IDLE,
    WAITING
};

class ControllerInAck : public ControllerIn {

    ControllerInState state;
    int lastServed;

    Element* e;
    vector<TypedWire<AckMessage>*> wires;

    public:

    ControllerInAck(Element* e);

    void step();
    void addWire(Wire* w);

};

class ControllerOutAck : public ControllerOut {

    ControllerOutState state;
    int lastServed;

    Element* e;
    vector<TypedWire<AckMessage>*> wires;

    public:

    ControllerOutAck(Element* e);
    void step();
    void addWire(Wire* w);
    
};

#endif
