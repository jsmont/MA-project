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

std::ostream& operator<<(std::ostream& os, AckMessage& f);

enum class AckControllerInState {
    IDLE,
    WAITING
};

enum class AckControllerOutState{
    IDLE,
    WAITING
};

class ControllerInAck : public ControllerIn {

    AckControllerInState state;
    int lastServed;

    Element* e;
    vector<TypedWire<AckMessage>*> wires;

    public:

    ControllerInAck(Element* e);

    void step();
    void addWire(Wire* w);

};

class ControllerOutAck : public ControllerOut {

    AckControllerOutState state;
    int lastServed;
    int cycles_waited;

    Element* e;
    vector<TypedWire<AckMessage>*> wires;

    public:

    ControllerOutAck(Element* e);
    void step();
    void addWire(Wire* w);

};

#endif
