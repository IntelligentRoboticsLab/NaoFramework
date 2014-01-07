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
        /**
         * @brief This class represents the core of the framework.
         *
         * This class has the task of managing all BrainWaves and Blackboards, and
         * offering an API to access them.
         */
        class Brain {
            public:
                using Inputs = std::vector<std::string>&;

                /**
                 * @brief Basic constructor.
                 */
                Brain();
                /**
                 * @brief This destructor stops all threads and correctly clears memory.
                 *
                 * The destructor clears all containers in the correct order as to avoid
                 * to unload shared library code which is supposed to delete created 
                 * instances residing within all Blackboards, resulting in segfaults.
                 */
                ~Brain();

                Brain(const Brain &) = delete;
                Brain & operator=(const Brain &) = delete;

                /**
                 * @brief This function checks whether a BrainWave with the given name exists.
                 *
                 * @param wave The name of the BrainWave.
                 *
                 * @return True if the BrainWave exists, false otherwise.
                 */
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

                /**
                 * @brief Unconditionally creates a BrainWave.
                 *
                 * @param s The name of the new BrainWave.
                 */
                void makeWave(const std::string & s);

                /**
                 * @brief This class extends Comm::ExternalBlackboardAdapterMap.
                 *
                 * This class implements the interface provided by Comm::ExternalBlackboardAdapterMap
                 * with respect to the data containers used in the Brain implementation.
                 */
                class ExternalBlackboardMap;
        };
    } // Core
} //NaoFramework

#endif
