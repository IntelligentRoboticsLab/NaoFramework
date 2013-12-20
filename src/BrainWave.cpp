#include <NaoFramework/Modules/BrainWave.hpp>

#include <NaoFramework/Modules/ModuleInterface.hpp>

namespace NaoFramework {
    namespace Modules {
        BrainWave::BrainWave(std::string name) : name_(name) {}

        BrainWave::~BrainWave() {
            for ( auto p : modules_ )
                delete p;
        }

        // Need to actually decide policy for adding/removing modules at runtime
        void BrainWave::addModule(ModuleInterface * module) {
            if ( isRunning() ) return;

            modules_.push_back(module);
            indices_[module->getName()] = modules_.size()-1;
        }

        void BrainWave::launchWave() {
            while ( running_.load(std::memory_order_relaxed) ) {
                for ( auto p : modules_ )
                    p->print(); // This is going to be something else.. need to decide module API
            }
        }

        void BrainWave::run() {
            if ( running_.load(std::memory_order_relaxed) ) return;

            running_.store(true, std::memory_order_relaxed);

            // Run thread
            wave_ = std::thread(&BrainWave::launchWave, this);
        }

        void BrainWave::pause() {
            if ( !running_.load(std::memory_order_relaxed) ) return;

            running_.store(false, std::memory_order_release);

            // Thread join
            wave_.join();
        }

        bool BrainWave::isRunning() const {
            return running_.load(std::memory_order_relaxed);
        }
    }
}
