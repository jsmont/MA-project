#include "Queue.h"
#include "utils.h"

Queue::Queue(string id, int size){
    this->size = size;
    this->id = id;
    this->ocupation = 0;
    this->next_ocupation = 0;
}

string Queue::getId(){
    return id;
}

bool Queue::request_push(){
    return ocupation < size;
}

bool Queue::request_pop(){
    return ocupation > 0;
}

void Queue::push(){
    if(ocupation < size)
        next_ocupation++;
    else 
        perror("Push to a full queue");
}

void Queue::pop(){
    if(ocupation > 0)
        next_ocupation--;
    else 
        perror("Pop to an empty queue");
}

void Queue::commit(){
    ocupation = next_ocupation;
}

stringstream Queue::report(){
    
    stringstream r;

    r << "Q: "  << id << "\tS: " << size << "\tO: " << ocupation << endl; 

    return r;
}
