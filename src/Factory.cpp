#include "Factory.h"
#include "ModeAck.h"
#include "ModeSCredited.h"

Factory::Factory(){
    this->config=FactoryConfiguration::Ack;
}

Factory::Factory(FactoryConfiguration config){
    this->config=config;
}

Node* Factory::node(string id, Element* e){

    Node* n = new Node();

    n->e = e;

    ControllerIn* in;
    ControllerOut* out;

    switch(config){
        case FactoryConfiguration::Ack:
            in = new ControllerInAck(e);
            out = new ControllerOutAck(e);
            break;
        case FactoryConfiguration::SCredited:
            bool* emited = new bool;
            *emited = false;
            in = new ControllerInSCredited(e, emited);
            out = new ControllerOutSCredited(e, emited);
            break;
    }

    
    n->in = in;
    n->out = out;

    return n;
    
}

Wire* Factory::wire(int latency, string srcId, string destId){

    Wire* w;

    switch(config){
        case FactoryConfiguration::Ack:
            w = (Wire*) new TypedWire<AckMessage>(latency, srcId, destId);
            break;
        case FactoryConfiguration::SCredited:
            w = (Wire*) new TypedWire<SCreditedMessage>(latency, srcId, destId);
            break;
    }

    return w;

}
