#ifndef __MODE_NACK_H__
#define __MODE_NACK_H__

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

enum class NackControllerInState {
    IDLE,
    WAITING
};

enum class NackControllerOutState{
    IDLE,
    WAITING
};

class ControllerInNack : public ControllerIn {

    NackControllerInState state;
    int lastServed;

    Element* e;
    vector<TypedWire<NackMessage>*> wires;

    public:

    ControllerInNack(Element* e);

    void step();
    void addWire(Wire* w);

};

class ControllerOutNack : public ControllerOut {

    NackControllerOutState state;
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
