#include "Core.h"
#include "Process.h"
#include "Queue.h"

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

    while( getline(spec, line) && !blankFound ){

        istringstream sline(line);
        
        string type;
        
        if(getline(sline, type, ':')){
            string id;
            sline >> id;

            Element* e;

            if(type == "P") {
                float mean, deviation;
                sline >> mean >> deviation;
                cout << "Processor found: " << id << "with mean: " << mean << " and deviation: " << deviation << endl;
                e = (Element *) new Process(id, mean, deviation);
            } else if(type == "Q") {
                int size;
                sline >> size;
                cout << "Queue found: " << id << " of size: " << size << endl;
                e = (Element *) new Queue(id, size);
            }

            Node* n = this->f.node(id, e);

            this->g.addNode(n);
            
        } else {
            blankFound = true;
        }
    }

}
