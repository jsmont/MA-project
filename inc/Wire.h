#ifndef __WIRE_H__
#define __WIRE_H__

#include "utils.h"

template <typename R>
struct WireState{
    R req_message;
    int req_pending_cycles;
    bool req;
    R answ_message;
    int answ_pending_cycles;
    bool answ;
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
    struct WireState<T> state;
    struct WireState<T> next_state;
    string src_id;
    string dest_id;
    public:

    TypedWire(int latency, string srcId, string destId);

    string getSrcId();
    string getDestId();

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

#endif
