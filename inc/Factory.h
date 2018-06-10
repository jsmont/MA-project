#ifndef __FACTORY_H__
#define __FACTORY_H__

#include "Node.h"
#include "Element.h"

enum class FactoryConfiguration {
    Ack
};

class Factory{

    FactoryConfiguration config;

    public:
    Factory();
    Factory(FactoryConfiguration config);

    Node* node(string id, Element* e);

    Wire* wire(int latency, string srcId, string destId);

};

#endif
