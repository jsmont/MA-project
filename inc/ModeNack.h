#ifndef __MODE_ACK_H__
#define __MODE_ACK_H__

#include "Element.h"
#include "Wire.h"
#include "ControllerIn.h"
#include "ControllerOut.h"
#include <vector>

enum class NackMessage{
    REQ,
    NACK
};

std::ostream& operator<<(std::ostream& os, NackMessage& f);

enum class ControllerInState {
    IDLE,
    WAITING
};

enum class ControllerOutState{
    IDLE,
    WAITING
};

class ControllerInNack : public ControllerIn {

    ControllerInState state;
    int lastServed;

    Element* e;
    vector<TypedWire<NackMessage>*> wires;

    public:

    ControllerInNack(Element* e);

    void step();
    void addWire(Wire* w);

};

class ControllerOutNack : public ControllerOut {

    ControllerOutState state;
    int lastServed;
    int cycles_waited;

    Element* e;
    vector<TypedWire<NackMessage>*> wires;

    public:

    ControllerOutNack(Element* e);
    void step();
    void addWire(Wire* w);

};

#endif
