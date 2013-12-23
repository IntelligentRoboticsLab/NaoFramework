#ifndef NAO_FRAMEWORK_CORE_BRAIN_WAVE_HEADER_FILE
#define NAO_FRAMEWORK_CORE_BRAIN_WAVE_HEADER_FILE

#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <memory>

namespace NaoFramework {
    namespace Modules { class ModuleInterface; }
    namespace Core {
        class BrainWave {
            public:
                BrainWave(std::string name);
                ~BrainWave();

                void addModule(std::unique_ptr<Modules::ModuleInterface> && module);

                void execute();
                void pause();
                bool isRunning() const;
            private:
                std::string name_;

                std::vector<std::unique_ptr<Modules::ModuleInterface>> modules_;
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
