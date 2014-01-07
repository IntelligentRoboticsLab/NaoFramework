#ifndef NAO_FRAMEWORK_CORE_BRAIN_WAVE_HEADER_FILE
#define NAO_FRAMEWORK_CORE_BRAIN_WAVE_HEADER_FILE

#include <NaoFramework/Log/Loggable.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <memory>

namespace NaoFramework {
    namespace Modules { class ModuleInterface; }
    namespace Core {
        /**
         * @brief This class manages a single thread of execution and its modules.
         *
         * This class manages the execution of a single thread, which loops continuously
         * over the modules added to it. In addition, all modules assigned to the 
         * BrainWave are also owned by it, and are thus cleared once the BrainWave
         * is destroyed.
         */
        class BrainWave : public Log::Loggable {
            public:
                using Module = std::unique_ptr<Modules::ModuleInterface>;

                /**
                 * @brief Basic constructor.
                 *
                 * @param name The name of the BrainWave.
                 */
                BrainWave(std::string name);

                ~BrainWave();
                /**
                 * @brief Move constructor.
                 *
                 * The move constructor temporarily pauses the wave for transport,
                 * as the internal thread uses the internal atomic flag in order
                 * to stop, and after moving it would reference the wrong place.
                 *
                 * @param wave The BrainWave that is being moved.
                 */
                BrainWave(BrainWave && wave);
                /**
                 * @brief Move assigment operator.
                 *
                 * The move assignment operator temporarily pauses the wave for transport,
                 * as the internal thread uses the internal atomic flag in order
                 * to stop, and after moving it would reference the wrong place.
                 *
                 * @param wave The BrainWave that is being moved.
                 *
                 * @return The BrainWave itself.
                 */
                const BrainWave & operator=(BrainWave && wave);

                /**
                 * @brief This function adds a module to the BrainWave.
                 *
                 * If the BrainWave is running, it will be stopped for insertion, and 
                 * then restarted. As long as dependencies are correct this should not 
                 * be a problem, as no Blackboard is touched. The new module is added 
                 * as the last entry of the current list of loaded modules, and will 
                 * be called last.
                 *
                 * @param module The module that is being acquired by the BrainWave.
                 */
                void addModule(Module && module);

                /**
                 * @brief This function starts the execution of the BrainWave.
                 */
                void execute();
                /**
                 * @brief This function stops the execution of the BrainWave.
                 */
                void pause();
                /**
                 * @brief This function checks whether the BrainWave is running.
                 *
                 * @return True if the BrainWave is running a thread, false otherwise.
                 */
                bool isRunning() const;

                /**
                 * @brief This function returns the name of the BrainWave.
                 *
                 * @return The name of the BrainWave.
                 */
                const std::string & getName() const;
            private:
                std::string name_;

                std::vector<Module> modules_;
                std::unordered_map<std::string, size_t> indices_;

                std::atomic<bool> running_;

                void launchWave();
                std::thread wave_;
        };
    } // Core
} //NaoFramework
#endif

/*
 *
 *
 * BrainWave b;
 * b.add(module);
 * b.add(module);
 * 
 * */
