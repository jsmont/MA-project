#ifndef __CONTROLLER_IN_H__
#define __CONTROLLER_IN_H__

#include "Wire.h"

class ControllerIn {
    public:
        virtual void step(){}
        virtual void addWire(Wire* w){}
};

#endif
