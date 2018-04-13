#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Core.h"

class Queue : virtual public Element {

    int id;
    int size;
    int ocupation;
    int next_ocupation;

public:

    Queue(int id, int size);

    bool request_push(); //Returns whether we can push or not.
    bool request_pop(); //Returns whether it has anything pending or not.

    void push(); //Add an element to the queue.
    void pop(); //Take an element from the queue.

    void commit(); //Uptade internal state for next cycle.

    stringstream report(); //Print its stats.

};


#endif
