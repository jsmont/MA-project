#include "ModeAck.h"
#include "utils.h"

ControllerInAck::ControllerInAck(Element* e){

    this->e = e;
    wires = vector<TypedWire<AckMessage>*>(0);
    state = ControllerInState::WAITING;
    lastServed = 0;

}

void ControllerInAck::step(){

    if(wires.size() != 0){
        bool served = false;
        switch(state){
            case ControllerInState::IDLE:
                for(int i = lastServed+1; i != lastServed && !served; i = (i +1) % wires.size()){
                    if(wires[i]->arrivedReq()){

                        AckMessage m = wires[i]->getReq();
                        e->push();
                        wires[i]->sendAnsw(AckMessage::ACK);

                        lastServed = i;
                        served=true;
                        state = ControllerInState::WAITING;
                    }
                }
                break;
            case ControllerInState::WAITING:
                if(e->request_push()){
                    state = ControllerInState::IDLE;
                    step();
                }
                break;
        }
    } else{
        if(e->request_push()) {
            e->push();
            D("Inserting new ticket at Root node");
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

    if(wires.size() != 0){

        bool served = false;
        switch(state){

            case ControllerOutState::IDLE:
                if(e->request_pop()){
                    for(int i = lastServed+1; i != lastServed && !served; i = (i+1)%wires.size()){
                        if(!wires[i]->req_busy()){
                            wires[lastServed]->sendReq(AckMessage::REQ);
                            state=ControllerOutState::WAITING;
                            served=true;
                            lastServed = i;
                        }
                    }
                }
                break;
            case ControllerOutState::WAITING:
                if(wires[lastServed]->arrivedAnsw()){
                    e->pop();
                    state=ControllerOutState::IDLE;

                }
                break;
        }

    } else { 
        if(e->request_pop()){
            e->pop();
            D("Ticket finished");
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

