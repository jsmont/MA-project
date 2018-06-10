#ifndef __CONTROLLER_OUT_H__
#define __CONTROLLER_OUT_H__

#include "Wire.h"

class ControllerOut {
    public:
        virtual void step(){}
        virtual void addWire(Wire* w){}
};
#endif
