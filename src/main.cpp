#include <iostream>
#include "utils.h"
#include "Factory.h"
#include "Core.h"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " GRAPH_FILE LOG_FILE [-m mode]" << std::endl;
        return 1;
    } 

	string graphF = string(argv[1]);
    string outF = string(argv[2]);

	FactoryConfiguration Fmode = FactoryConfiguration::Ack;

	if(argc > 3) {
		for(int i = 3; i < argc; i+=2){
			if(string(argv[i]) == "--f"){
				if(string(argv[i+1]) == "ack") {
					Fmode = FactoryConfiguration::Ack;
                    std::cout << "Fmode set to: Ack" << std::endl;
				}
                else if(string(argv[i+1]) == "scredit") {
					Fmode = FactoryConfiguration::SCredited;
                    std::cout << "Fmode set to: SCredited" << std::endl;
				}
			}
		}
	}

    ofstream log;
    log.open(outF);

	Factory f = Factory(Fmode);
	
	Core c = Core(f);
	
	c.parse(graphF);
    
    //Skip transitive part
    log << c.simulate(1000000).str() << endl;
    log.close();

}
