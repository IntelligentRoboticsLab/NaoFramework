#include <NaoFramework/Core/Brain.hpp>
#include <NaoFramework/Modules/DynamicModule.hpp>
//#include <NaoFramework/Log/Frontend.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>
//#include <NaoFramework/Comm/ExternalBlackboardAdapter.hpp>
#include <NaoFramework/Comm/LocalBlackboardAdapter.hpp>

#include <iostream>

using std::cout;

// TODO: Make this into a class and its own file; put it in the include/s
namespace NaoFramework {
    namespace Core {
        Brain::Brain() : testWave_("Wave"), b("BBoard") {}
        Brain::~Brain() {
            testWave_.pause();
        }

        unsigned Brain::addDynamicModule(std::vector<std::string>& inputs) {
            if ( inputs.size() < 2 ) {
                std::cout << "Usage: " << inputs[0] << " module_filename\n";
                return 1;
            }

            unsigned loaded = 1; // 1 = Error!
            try {
                auto adapter = NaoFramework::Comm::LocalBlackboardAdapter(b);
                auto dynModule = NaoFramework::Modules::makeDynamicModule(inputs[1], adapter);
                std::cout << "Successfully loaded module: " << dynModule->getName() << "\n";
                testWave_.addModule(std::move(dynModule)); // Give ownership
                // Here dynModule is empty! If we want to print here, we need to add some
                // functions to BWave first (which we'll need to add anyway)
                loaded = 0;
            }
            catch ( std::runtime_error & e ) {
                std::cout << "Could not load module: " << e.what() << "\n";
            }
            return loaded;
        }

        unsigned Brain::execute(std::vector<std::string>&) {
            if ( ! b.validateGlobals() ) {
                std::cout << "Dependencies are not met!\n";
                // We should give out better info on what is wrong. BBoard, save us!
                return 1;
            }
            // Launch thread
            testWave_.execute();
            std::cout << "Waves successfully started in the background!\n";
            return 0;
        }
    }
}
