#include "ModeNack.h"
#include "utils.h"

std::ostream& operator<<(std::ostream& os, NackMessage& f){
    string id;
    switch(f){
        case NackMessage::REQ :
            id = "REQ";
            break;
        case NackMessage::NACK :
            id = "NACK";
            break;
        default:
            id = "NULL";
    }
    os << id;
}

ControllerInNack::ControllerInNack(Element* e){

    this->e = e;
    wires = vector<TypedWire<NackMessage>*>(0);
    state = ControllerInState::WAITING;
    lastServed = 0;

}

void ControllerInNack::step(){

    if(wires.size() != 0){
        int initial_i, i;
        initial_i = i  = (lastServed+1)%wires.size();
        do{
            if(wires[i]->arrivedReq()){

                NackMessage m = wires[i]->getReq();
                if(!e->request_push()){
                    wires[i]->sendAnsw(NackMessage::NACK);
                }

                lastServed = i;
                state = ControllerInState::WAITING;
            }
            i = (i +1) % wires.size();
        } while(i != initial_i);
    } else{
        if(e->request_push()) {
            e->push();
            //D("Inserting new ticket at Root node");
        }
    }
}

void ControllerInNack::addWire(Wire* w){

    TypedWire<NackMessage>* tw = (TypedWire<NackMessage>*) w;

    assert(tw->getDestId() == e->getId());

    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getSrcId() != tw->getSrcId());

    wires.push_back(tw);

}

/////////////////////////////////////////////////////
//   CONTROLLER OUT                                //
/////////////////////////////////////////////////////

ControllerOutNack::ControllerOutNack(Element* e){
    this->e = e;
    wires = vector<TypedWire<NackMessage>*>(0);

    state = ControllerOutState::IDLE;
    lastServed = 0;
}


void ControllerOutNack::step(){
    
    //D("Controller step: " << e->getId());
    //D("Step: " << e->getId() << "\tW: " << wires.size());

    if(wires.size() != 0){

        bool served = false;
        //if(state == ControllerOutState::IDLE) D("S: IDLE");
        //if(state == ControllerOutState::WAITING) D("S: WAITING");
        switch(state){

            case ControllerOutState::IDLE:
                if(e->request_pop()){
                    //D("Trying to serve ticket N:" << e->getId())
                        for(int i = (lastServed+1)%wires.size(); i != lastServed && !served; i = (i+1)%wires.size()){
                            if(!wires[i]->req_busy()){
                                wires[i]->sendReq(NackMessage::REQ);
                                state=ControllerOutState::WAITING;
                                served=true;
                                lastServed = i;
                            }
                        }
                    if(!served){
                        if(!wires[lastServed]->req_busy()){
                            wires[lastServed]->sendReq(NackMessage::REQ);
                            state=ControllerOutState::WAITING;
                            served=true;
                        }
                    }
                }
                break;
            case ControllerOutState::WAITING:
                if(wires[lastServed]->arrivedAnsw()){
                    assert(cycles_waited == (2*wires[lastServed]->getLatency()+1));
                    NackMessage answer = wires[lastServed]->getAnsw();
                    state=ControllerOutState::IDLE;
                } else {
                    if(cycles_waited == (2*wires[lastServed]->getLatency() +1)){
                        e->pop();
                        state=ControllerOutState::IDLE;
                    }
                    else{
                        cycles_waited++;
                    }
                }
                break;
        }

    } else { 
        if(e->request_pop()){
            e->pop();
            //D("Ticket finished");
        }
    }

}


void ControllerOutNack::addWire(Wire* w){

    TypedWire<NackMessage>* tw = (TypedWire<NackMessage>*) w;

    assert(tw->getSrcId() == e->getId());

    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getDestId() != tw->getSrcId());

    wires.push_back(tw);

}

