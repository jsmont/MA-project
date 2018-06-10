#include "Factory.h"
#include "ModeAck.h"

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
    }

    n->in = in;
    n->out = out;

    return n;
    
}


