#ifndef __MODE_S_CREDITED__
#define __MODE_S_CREDITED__

#include "Element.h"
#include "Wire.h"
#include "ControllerIn.h"
#include "ControllerOut.h"
#include <vector>
#include <queue>

enum class SCreditedMessage {
    REQ,
    TKN
};



std::ostream& operator<<(std::ostream& os, SCreditedMessage& f);

class ControllerInSCredited : public ControllerIn {

    vector<int> credits;
    Element* e;
    bool* emited;

    queue<int> queued;

    vector<TypedWire<SCreditedMessage>*> wires;
    
    public:

    ControllerInSCredited(Element* e, bool* emited);

    void step();
    void addWire(Wire* w);
};

class ControllerOutSCredited : public ControllerOut {

    vector<int> credits;
    int lastServed;
    
    Element* e;
    bool* emited;

    vector<TypedWire<SCreditedMessage>*> wires;

    public:
    ControllerOutSCredited(Element* e, bool* emited);

    void step();
    void addWire(Wire* w);

};


#endif
