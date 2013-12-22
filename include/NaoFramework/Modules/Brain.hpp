#ifndef NAO_FRAMEWORK_MODULES_WAVE_CONDUCTOR_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_WAVE_CONDUCTOR_HEADER_FILE

#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>

#include <string>
#include <vector>

namespace NaoFramework {
    namespace Modules {
        class Brain {
            public:
                //Brain() {}
                //Brain(const Brain &) = delete;
                //Brain & operator=(const Brain &) = delete;
                Brain();

                unsigned addModule(std::vector<std::string>& inputs);
                unsigned printModules(std::vector<std::string>&);
            private:
                std::vector<DynamicModule> sharedLibs;
                NaoFramework::Comm::Blackboard b;
        };
    }
}

#endif
