#ifndef __CORE_H__
#define __CORE_H__

#include "Graph.h"
#include "Factory.h"
#include "utils.h"
#include <fstream>

class Core {
    Graph g;
    Factory f;

    public:
    
    Core(Factory f);

    void parse(string path);

    stringstream simulate(int cycles);

    private:

    void getAndAddNodes(ifstream &spec);
    void getAndAddWires(ifstream &spec);

};

#endif
