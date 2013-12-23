#ifndef NAO_FRAMEWORK_CORE_BRAIN_HEADER_FILE
#define NAO_FRAMEWORK_CORE_BRAIN_HEADER_FILE

#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>
#include <NaoFramework/Core/BrainWave.hpp>

#include <string>
#include <vector>

namespace NaoFramework {
    namespace Core {
        class Brain {
            public:
                Brain();
                ~Brain();
                Brain(const Brain &) = delete;
                Brain & operator=(const Brain &) = delete;

                // These are the functions added by the Console
                // to give the API of the framework.
                unsigned addDynamicModule(std::vector<std::string>& inputs);
                unsigned execute(std::vector<std::string>&);
            private:
                // Testing, this should be a map of waves, but final
                // setup we still have to set
                BrainWave testWave_;
                NaoFramework::Comm::Blackboard b;
        };
    } // Core
} //NaoFramework

#endif
