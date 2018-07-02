#include "ModeSCredited.h"
#include "utils.h"
#include <iostream>

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
    if(wires.size() != 0){
        if(*emited){
            *emited=false;
            if(queued.empty()) std::cout << "[" << e->getId() << "]" << endl;
            assert(!queued.empty());
            credits[queued.front()]++;
            queued.pop();
        }

        for(int i = 0; i < credits.size(); ++i){
            if(credits[i] > 0){
                //D("[" << e->getId() << "] " << credits[i] << " credits pending for wire: " << i);
                if(!wires[i]->answ_busy()){
                    //D("[" << e->getId() << "] Credit sent for wire: " << i);
                    wires[i]->sendAnsw(SCreditedMessage::TKN);
                    credits[i]--;
                }
            }
            if(wires[i]->arrivedReq()){
                queued.push(i);
                e->push();
            }
        }
    } else {
        if(e->request_push()) {
            e->push();
            //D("Inserting new ticket at Root node: " << e->getId());
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

    assert(tw->getSrcId() == e->getId());

    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getDestId() != tw->getSrcId());

    wires.push_back(tw);

    credits = vector<int>(wires.size(), 0);
}

void ControllerOutSCredited::step(){


    if(wires.size() != 0){
        if(e->request_pop()){
            //D("[" << e->getId() << "] Pending job to be sent");
            bool have_credits = false;
            int i = lastServed; 
            do {
                i = (i + 1) % wires.size();
                have_credits = ((credits[i] > 0) && (!wires[i]->req_busy()));
            } while (!have_credits && (i != lastServed));

            if(have_credits){
                //D("[" << e->getId() << "] Job sent to wire: " << i  << " pending credits: " << credits[i]);
                lastServed = i;
                wires[i]->sendReq(SCreditedMessage::REQ);
                *emited = true;
                credits[i]--;
                e->pop();
            }

        }

        for(int i = 0; i < wires.size(); ++i){
            if(wires[i]->arrivedAnsw()){
                //D("[" << e->getId() << "] Credit recieved for wire: " << i);
                credits[i]++;
            }
        }
    } else {
        if(e->request_pop()){
            e->pop();
            *emited=true;
            //D("Ticket finished");
        }
    }
}
