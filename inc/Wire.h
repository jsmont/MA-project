#ifndef __WIRE_H__
#define __WIRE_H__

#include "utils.h"
#include <queue>

template <typename R>
struct WireState{
    R message;
    bool valid;
};


class Wire{
    public:
        virtual string getSrcId(){}
        virtual string getDestId(){}
        virtual void commit(){}
        virtual stringstream report(){}
};

template <class T>
class TypedWire : public Wire{
    int latency;
    std::queue<WireState<T> > req;
    std::queue<WireState<T> > answ;
    bool pushed_req;
    bool pushed_answ;
    string src_id;
    string dest_id;
    public:

    TypedWire(int latency, string srcId, string destId);

    string getSrcId();
    string getDestId();
    int getLatency();

    bool req_busy();
    bool answ_busy();
    
    bool arrivedReq();
    T getReq();
    void sendReq(T message);

    bool arrivedAnsw();
    T getAnsw();
    void sendAnsw(T message);

    void commit();
    stringstream report(); //Print its stats.
};

template <class T>
TypedWire<T>::TypedWire(int latency, string srcId, string destId){
    this->latency = latency;
    src_id = srcId;
    dest_id = destId;
    req = queue<WireState<T> >();
    answ = queue<WireState<T> >();
    for(int i = 0; i <= latency; ++i){
        struct WireState<T> r,a;
        r.valid = false;
        a.valid = false;
        req.push(r);
        answ.push(a);
    }
    pushed_req = false;
    pushed_answ = false;
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
int TypedWire<T>::getLatency(){
    return latency;
}

template <class T>
bool TypedWire<T>::req_busy(){
    return pushed_req;
}

template <class T>
bool TypedWire<T>::answ_busy(){
    return pushed_answ;
}

template <class T>
bool TypedWire<T>::arrivedReq(){
    assert(!req.empty());
    return req.front().valid;
}

template <class T>
T TypedWire<T>::getReq(){
    assert(!req.empty());
    if(req.front().valid){
        return req.front().message;
    } else
        perror("Tried to read a request on a wire without a request");
    return req.front().message;
}

template <class T>
void TypedWire<T>::sendReq(T message){
    if(!pushed_req){
        struct WireState<T> r;
        r.valid = true;
        r.message = message;
        req.push(r);
        pushed_req = true;
    } else perror("Tried to send a req on a busy wire");
}

template <class T>
bool TypedWire<T>::arrivedAnsw(){
    assert(!answ.empty());
    return answ.front().valid;
}

template <class T>
T TypedWire<T>::getAnsw(){
    assert(!answ.empty());
    if(answ.front().valid){
            return answ.front().message;
    } else
        perror("Tried to read an answer on a wire without an answer");
    return answ.front().message;
}

template <class T>
void TypedWire<T>::sendAnsw(T message){
    if(!pushed_answ){
        struct WireState<T> a;
        a.valid = true;
        a.message = message;
        answ.push(a);
        pushed_answ = true;
    } else perror("Tried to send an answer on a busy wire");
}

template <class T>
void TypedWire<T>::commit(){

    //D("Commiting wire " << this->src_id << "->" << this->dest_id);

    req.pop();
    answ.pop();
    if(!pushed_req){
        struct WireState<T> r;
        r.valid = false;
        req.push(r);
    }
    if(!pushed_answ){
        struct WireState<T> r;
        r.valid = false;
        answ.push(r);
    }

}

template <class T>
stringstream TypedWire<T>::report(){
    stringstream r;
    /*
    r << "W: " << this->src_id << "->" << this->dest_id << "\tR: " << this->state.req << "\tM: " << this->state.req_message;
    r <<"\tP: " << this->state.req_pending_cycles << "\tA: " << this->state.answ << "\tM: " << this->state.answ_message;
    r << "\tP:" << this->state.answ_pending_cycles << endl;
    //D("Wire reported");
    */

    if(pushed_req)
        r << "<REQ>" << endl;

    if(pushed_answ)
        r << "<ANSW>" << endl;

    pushed_req = false;
    pushed_answ = false;

    return r;
}
#endif
