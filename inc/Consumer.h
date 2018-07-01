#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include "Process.h"
#include "Queue.h"
#include "utils.h"

class Consumer : virtual public Element {

    string id;
    Process p;
    Queue q;

    public:

    Consumer(string id, float mean, float std, int size);

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
