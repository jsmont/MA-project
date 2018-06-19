#include "ModeSCredited.h"

std::ostream& operator<<(std::ostream& os, SCreditedMessage& f){
    string id = "";
    switch(f){
        case SCreditedMessage::REQ:
            id="REQ";
            break;
        case SCreditedMessage::TKN:
            id="TKN";
            break;
        default:
            id="NULL";
    }

    return os << id;
}

ControllerInSCredited::ControllerInSCredited(Element* e, bool* emited){
    this->e = e;
    this->emited = emited;
    this->credits = vector<int>(0,0);
    
    this->wires = vector<TypedWire<SCreditedMessage>*>(0); 
}

void ControllerInSCredited::addWire(Wire* w){
    TypedWire<SCreditedMessage>* tw = (TypedWire<SCreditedMessage>*)w;

    assert(tw->getDestId() == e->getId());

    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getSrcId() != tw->getSrcId());

    wires.push_back(tw);

    credits = vector<int>(wires.size(), e->getCredits()/wires.size());
}

void ControllerInSCredited::step(){
    if(*emited){
        *emited=false;
        assert(!queued.empty());
        credits[queued.front()]++;
    }

    for(int i = 0; i < credits.size(); ++i){
        if(credits[i] > 0){
            if(!wires[i]->answ_busy()){
                wires[i]->sendAnsw(SCreditedMessage::TKN);
                credits[i]--;
            }
        }
        if(wires[i]->arrivedReq()){
            queued.push(i);
            e->push();
        }
    }
}

ControllerOutSCredited::ControllerOutSCredited(Element* e, bool* emited){

    this->e = e;
    this->emited = emited;

    this->wires = vector<TypedWire<SCreditedMessage>*>(0);
    this->credits = vector<int>(0);

}

void ControllerOutSCredited::addWire(Wire* w){
    TypedWire<SCreditedMessage>* tw = (TypedWire<SCreditedMessage>*)w;

    D("Out");
    assert(tw->getSrcId() == e->getId());

    D("Out");
    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getDestId() != tw->getSrcId());

    D("Out");
    wires.push_back(tw);

    D("Out");
    credits = vector<int>(wires.size(), 0);
}

void ControllerOutSCredited::step(){

    if(e->request_pop()){
        bool have_credits = false;
        int i = lastServed; 
        do {
            i = (i + 1) % wires.size();
            have_credits = ((credits[i] > 0) && (!wires[i]->req_busy()));
        } while (!have_credits && (i != lastServed));

        if(have_credits){
            lastServed = i;
            wires[i]->sendReq(SCreditedMessage::REQ);
            *emited = true;
            credits[i]--;
        }

    }

    for(int i = 0; i < wires.size(); ++i){
        if(wires[i]->arrivedAnsw()){
            credits[i]++;
        }
    }

}
