#include "Core.h"

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

            if(type == "P") {
                cout << "Processor found: " << id << endl;
            } else if(type == "Q") {
                cout << "Queue found: " << id << endl;
            }
            
        } else {
            blankFound = true;
        }
    }

}
