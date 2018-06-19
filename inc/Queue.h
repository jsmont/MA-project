#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Element.h"

class Queue : virtual public Element {

    string id;
    int size;
    int ocupation;
    int push_count;
    int pop_count;

public:

    Queue(string id, int size);

    string getId();
    int getCredits();

    bool request_push(); //Returns whether we can push or not.
    bool request_pop(); //Returns whether it has anything pending or not.

    void push(); //Add an element to the queue.
    void pop(); //Take an element from the queue.

    void commit(); //Uptade stringernal state for next cycle.

    stringstream report(); //Print its stats.

};


#endif
