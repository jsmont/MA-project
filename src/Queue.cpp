#include "Queue.h"
#include "utils.h"

Queue::Queue(){
}

Queue::Queue(string id, int size){
    this->size = size;
    this->id = id;
    this->ocupation = 0;
    this->push_count = 0;
    this->pop_count = 0;
}

string Queue::getId(){
    return id;
}

int Queue::getCredits(){
    return this->size;
}

bool Queue::request_push(){
    return ocupation + push_count < size;
}

bool Queue::request_pop(){
    return ocupation - pop_count > 0;
}

void Queue::push(){
    if(ocupation + push_count < size)
        push_count++;
    else 
        perror("Push to a full queue");
}

void Queue::pop(){
    if(ocupation - pop_count > 0)
        pop_count++;
    else 
        perror("Pop to an empty queue");
}

void Queue::commit(){
    ocupation = ocupation + push_count - pop_count;
    push_count = pop_count = 0;
}

stringstream Queue::report(){
    
    stringstream r;

     //D( "Q: "  << id << "\tS: " << size << "\tO: " << ocupation);

    return r;
}
