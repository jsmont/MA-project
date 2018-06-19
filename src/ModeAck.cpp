#include "ModeAck.h"
#include "utils.h"

std::ostream& operator<<(std::ostream& os, AckMessage& f){
    string id;
    switch(f){
        case AckMessage::REQ :
            id = "REQ";
            break;
        case AckMessage::ACK :
            id = "ACK";
            break;
        case AckMessage::NACK :
            id = "NACK";
        default:
            id = "NULL";
    }
    os << id;
}

ControllerInAck::ControllerInAck(Element* e){

    this->e = e;
    wires = vector<TypedWire<AckMessage>*>(0);
    state = ControllerInState::WAITING;
    lastServed = 0;

}

void ControllerInAck::step(){

    if(wires.size() != 0){
        int initial_i, i;
        initial_i = i  = (lastServed+1)%wires.size();
        do{
            if(wires[i]->arrivedReq()){

                AckMessage m = wires[i]->getReq();
                if(e->request_push()){
                    e->push();
                    wires[i]->sendAnsw(AckMessage::ACK);
                } else {
                    wires[i]->sendAnsw(AckMessage::NACK);
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

void ControllerInAck::addWire(Wire* w){

    TypedWire<AckMessage>* tw = (TypedWire<AckMessage>*) w;

    assert(tw->getDestId() == e->getId());

    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getSrcId() != tw->getSrcId());

    wires.push_back(tw);

}

/////////////////////////////////////////////////////
//   CONTROLLER OUT                                //
/////////////////////////////////////////////////////

ControllerOutAck::ControllerOutAck(Element* e){
    this->e = e;
    wires = vector<TypedWire<AckMessage>*>(0);

    state = ControllerOutState::IDLE;
    lastServed = 0;
}


void ControllerOutAck::step(){
    
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
                                wires[i]->sendReq(AckMessage::REQ);
                                state=ControllerOutState::WAITING;
                                served=true;
                                lastServed = i;
                            }
                        }
                    if(!served){
                        if(!wires[lastServed]->req_busy()){
                            wires[lastServed]->sendReq(AckMessage::REQ);
                            state=ControllerOutState::WAITING;
                            served=true;
                        }
                    }
                }
                break;
            case ControllerOutState::WAITING:
                if(wires[lastServed]->arrivedAnsw()){
                    AckMessage answer = wires[lastServed]->getAnsw();
                    if(answer == AckMessage::ACK)
                        e->pop();
                    state=ControllerOutState::IDLE;
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


void ControllerOutAck::addWire(Wire* w){

    TypedWire<AckMessage>* tw = (TypedWire<AckMessage>*) w;

    assert(tw->getSrcId() == e->getId());

    for(int i = 0; i < wires.size(); ++i)
        assert(wires[i]->getDestId() != tw->getSrcId());

    wires.push_back(tw);

}

