#include "Core.h"
#include "Process.h"
#include "Queue.h"
#include "utils.h"

Core::Core(Factory f){

    this->f = f;
    this->g = Graph();

}

void Core::parse(string path){

    ifstream spec;

    spec.open(path, ios::in);

    if(!spec.is_open()) perror("Specification file could not be opened.\n");

    getAndAddNodes(spec);
    //getAndAddWires(spec);

}

void Core::getAndAddNodes(ifstream &spec){

    bool blankFound = false;
    string line;

    D("Parsing elements");

    while( !blankFound && getline(spec, line) ){

        istringstream sline(line);
        
        string type;
        
        if(getline(sline, type, ':')){
            string id;
            sline >> id;

            Element* e;

            if(type == "P") {
                float mean, deviation;
                sline >> mean >> deviation;
                D("Processor found: " << id << " with mean: " << mean << " and deviation: " << deviation);
                e = (Element *) new Process(id, mean, deviation);
            } else if(type == "Q") {
                int size;
                sline >> size;
                D("Queue found: " << id << " of size: " << size);
                e = (Element *) new Queue(id, size);
            }

            Node* n = this->f.node(id, e);

            this->g.addNode(n);
            
        } else {
            blankFound = true;
        }
    }

    D("Parsing wires");

    while( getline(spec, line) ){

        istringstream sline(line);

        string sourceId;

        if(getline(sline, sourceId, ':')){
            
            string destId;
            int latency;

            sline >> destId >> latency;

            D("Wire found from " << sourceId << " to " << destId << " with latency " << latency);

            Wire* w = this->f.wire(latency, sourceId, destId);
            this->g.addWire(w);

        }


    }

}

stringstream Core::simulate(int cycles){

    stringstream log;

    for(int i = 0; i < cycles; ++i){
        log << "[" << i << "]" << endl;
        this->g.step();
        log << this->g.report().str();
    }

    return log;

}
