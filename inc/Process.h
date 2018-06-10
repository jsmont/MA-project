#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "Element.h"
#include <random>

typedef struct {
    int pending_cycles;
    bool free;
} ProcessState;

class Process : virtual public Element {

    string id;
    float mean;
    float std;
    std::normal_distribution<float> latency;

    ProcessState state;
    ProcessState next_state;

public:

    Process(string id, float mean, float std);

    string getId();

    bool request_push(); //Returns whether we can push or not.
    bool request_pop(); //Returns whether it has anything pending or not.

    void push(); //Add an element to the queue.
    void pop(); //Take an element from the queue.

    void commit(); //Uptade stringernal state for next cycle.

    stringstream report(); //Print its stats.

};


#endif
