#ifndef NAO_FRAMEWORK_CORE_BRAIN_HEADER_FILE
#define NAO_FRAMEWORK_CORE_BRAIN_HEADER_FILE

#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>
#include <NaoFramework/Core/BrainWave.hpp>

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

namespace NaoFramework {
    namespace Core {
        class Brain {
            public:
                using Inputs = std::vector<std::string>&;

                Brain();
                ~Brain();

                Brain(const Brain &) = delete;
                Brain & operator=(const Brain &) = delete;

                bool waveExists(const std::string & wave) const;

                // These are the functions added by the Console
                // to give the API of the framework.
                unsigned createWave         (Inputs inputs);
                unsigned addDynamicModule   (Inputs inputs);
                unsigned execute            (Inputs inputs);
            private:
                using BlackboardList = std::list<Comm::Blackboard>;
                BlackboardList blackboards_;
                std::unordered_map<std::string,std::pair<BrainWave, BlackboardList::iterator>> waves_;

                void makeWave(const std::string &);

                class ExternalBlackboardMap;
        };
    } // Core
} //NaoFramework

#endif
