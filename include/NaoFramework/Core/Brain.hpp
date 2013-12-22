#ifndef NAO_FRAMEWORK_CORE_BRAIN_HEADER_FILE
#define NAO_FRAMEWORK_CORE_BRAIN_HEADER_FILE

#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>

#include <string>
#include <vector>

namespace NaoFramework {
    namespace Core {
        class Brain {
            public:
                Brain();
                Brain(const Brain &) = delete;
                Brain & operator=(const Brain &) = delete;

                unsigned addModule(std::vector<std::string>& inputs);
                unsigned printModules(std::vector<std::string>&);
            private:
                std::vector<DynamicModule> sharedLibs;
                NaoFramework::Comm::Blackboard b;
        };
    } // Core
} //NaoFramework

#endif
