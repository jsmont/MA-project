#include <iostream>
#include "utils.h"
#include "Factory.h"
#include "Core.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " GRAPH_FILE [-m mode]" << std::endl;
        return 1;
    } 

	string graphF = string(argv[1]);
	FactoryConfiguration Fmode = FactoryConfiguration::Ack;

	if(argc > 2) {
		for(int i = 2; i < argc; i+=2){
			if(string(argv[i]) == "--f"){
				if(string(argv[i+1]) == "ack") {
					Fmode = FactoryConfiguration::Ack;
                    std::cout << "Fmode set to: Ack" << std::endl;
				}
			}
		}
	}

	Factory f = Factory(Fmode);
	
	Core c = Core(f);
	
	c.parse(graphF);

}
