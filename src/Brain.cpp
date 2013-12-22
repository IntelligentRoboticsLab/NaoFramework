#include <NaoFramework/Core/Brain.hpp>
#include <NaoFramework/Modules/DynamicModule.hpp>
//#include <NaoFramework/Console/Console.hpp>
//#include <NaoFramework/Log/Frontend.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>
//#include <NaoFramework/Comm/ExternalBlackboardAdapter.hpp>
#include <NaoFramework/Comm/LocalBlackboardAdapter.hpp>

//#include <map>
#include <vector>
#include <string>
//#include <iostream>

using std::cout;

// TODO: Make this into a class and its own file; put it in the include/s
namespace NaoFramework {
    namespace Core {
        Brain::Brain() : b("BBoard") {}

        unsigned Brain::addModule(std::vector<std::string>& inputs) {
            sharedLibs.reserve(5);
            if ( inputs.size() < 2 ) {
                std::cout << "Usage: " << inputs[0] << " module_filename\n";
                return 1;
            }

            unsigned loaded = 1; // 1 = Error!
            try {
                auto adapter = NaoFramework::Comm::LocalBlackboardAdapter(b);
                sharedLibs.emplace_back(NaoFramework::Modules::makeDynamicModule(inputs[1], adapter));
                std::cout << "Successfully loaded module: " << sharedLibs.back().getName() << "\n";
                loaded = 0;
            }
            catch ( std::runtime_error & e ) {
                std::cout << "Could not load module: " << e.what() << "\n";
            }
            return loaded;
        }

        unsigned Brain::printModules(std::vector<std::string>&) {
            if ( ! b.validateGlobals() ) {
                std::cout << "Dependencies are not met!\n";
                return 1;
            }
            for ( auto & module : sharedLibs ) {
                module.execute();
            }
            return 0;
        }
    }
}
