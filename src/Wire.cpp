#include "Wire.h"


template <class T>
TypedWire<T>::TypedWire(int latency, string srcId, string destId){
    this->latency = latency;
    state.busy = false;
    next_state.busy = false;
    src_id = srcId;
dest_id = destId;
}

template <class T>
string TypedWire<T>::getSrcId(){
    return src_id;
}

template <class T>
string TypedWire<T>::getDestId(){
    return dest_id;
}

template <class T>
bool TypedWire<T>::req_busy(){
    return state.req;
}

template <class T>
bool TypedWire<T>::answ_busy(){
    return state.answ;
}

template <class T>
bool TypedWire<T>::arrivedReq(){
    return state.req && (state.req_pending_cycles == 0);
}

template <class T>
T TypedWire<T>::getReq(){
    if(state.req){
        if(state.req_pending_cycles == 0){
            return state.req_message;
        } else
            perror("Tried to read an unfinished message");
    } else
        perror("Tried to read a request on a wire without a request");
    return state.req_message;
}

template <class T>
void TypedWire<T>::sendReq(T message){
    if(!state.req){
        next_state.req_message = message;
        next_state.req_pending_cycles = latency;
        next_state.req = true;
    } else perror("Tried to send a req on a busy wire");
}

template <class T>
bool TypedWire<T>::arrivedAnsw(){
    return !state.answ && (state.answ_pending_cycles == 0);
}

template <class T>
T TypedWire<T>::getAnsw(){
    if(state.answ){
        if(state.answ_pending_cycles == 0){
            return state.answ_message;
        } else
            perror("Tried to read an unfinished message");
    } else
        perror("Tried to read an answer on a wire without an answer");
    return state.answ_message;
}

template <class T>
void TypedWire<T>::sendAnsw(T message){
    if(!state.answ){
        next_state.answ_message = message;
        next_state.ans_pending_cycles = latency;
        next_state.answ_req = true;
    } else perror("Tried to send an answer on a busy wire");
}

template <class T>
void TypedWire<T>::commit(){

    state = next_state;

    if(next_state.answ){
        if(next_state.answ_pending_cycles > 0) next_state.answ_pending_cycles--;
        else next_state.answ = false;
    }

    if(next_state.req){
        if(next_state.req_pending_cycles > 0) next_state.req_pending_cycles--;
        else next_state.req = false;
    }

}

template <class T>
stringstream TypedWire<T>::report(){
    stringstream r;
    r << "W: " << src_id << "->" << dest_id << "\tR: " << state.req << "\tM: " << state.req_message << "\tP:" << state.req_pending_cycles << "\t:A: " << state.answ << "\tM: " << state.answ_message << "\tP:" << state.answ_pending_cycles << endl;

    return r;
}
