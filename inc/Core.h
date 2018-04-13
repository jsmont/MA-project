#ifndef _CORE_H_
#define _CORE_H_
#include "utils.h"

class Element {
    public:
    virtual bool request_push(); //Returns whether we can push or not.
    virtual bool request_pop(); //Returns whether it has anything pending or not.

    virtual void push(); //Add an element to the queue.
    virtual void pop(); //Take an element from the queue.

    virtual void commit(); //Uptade internal state for next cycle.
    virtual stringstream report(); //Print its stats.
};

#endif
