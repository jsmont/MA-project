#include "Process.h"
#include "utils.h"

Process::Process(){
}

Process::Process(string id, float mean, float std){

    this->id = id;
    this->mean = mean;
    this->std = std;
    latency = std::normal_distribution<float>(mean, std);
    state.free = true;
    state.pending_cycles = 0;
    finished= false;

    next_state.free = true;
    next_state.pending_cycles = 0;
}

string Process::getId(){
    return id;
}

int Process::getCredits(){
    return 1;
}

bool Process::request_push(){
    return state.free;
}

bool Process::request_pop(){
    return !state.free && (state.pending_cycles == 0);
}

void Process::push(){
    if(state.free){
        next_state.free = false;
        std::default_random_engine generator;
        next_state.pending_cycles = latency(generator);
    } else
        perror("Push to a busy node");
}

void Process::pop(){
    if(state.pending_cycles == 0){
        next_state.free = true;
    } else
        perror("Pop to an unfinished node");
}

void Process::commit(){
    finished = ((next_state.pending_cycles == 0 && state.pending_cycles != 0) || (!next_state.free && state.free && next_state.pending_cycles == 0));
    state = next_state;
    if(next_state.pending_cycles > 0) next_state.pending_cycles--;
}

stringstream Process::report(){

    stringstream r;

    //r << "N: " << id << "\tB: " << !state.free << "\tP:" << state.pending_cycles << endl;
    if(finished){
        r << "<" << id << ">" << endl;
    }

    return r;
}
