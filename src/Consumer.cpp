#include "Consumer.h"

Consumer::Consumer(string id, float mean, float std, int size){
    this->p = Process(id, mean, std);
    this->q = Queue(id, size);
    this->id = id;
}

string Consumer::getId(){
    return this->id;
}

int Consumer::getCredits(){
    return this->q.getCredits();
}

bool Consumer::request_push(){
    return this->q.request_push();
}

bool Consumer::request_pop(){
    return this->p.request_pop();
}

void Consumer::push(){
    this->q.push();
}

void Consumer::pop(){
    this->p.pop();
}

void Consumer::commit(){
    if(this->p.request_push() && this->q.request_pop()){
        this->q.pop();
        this->p.push();
    }

    this->q.commit();
    this->p.commit();
}

stringstream Consumer::report(){
    stringstream r;
    r << this->q.report().str();
    r << this->p.report().str();
    return r;
}
