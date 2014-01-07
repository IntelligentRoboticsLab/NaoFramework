#include <NaoFramework/Core/BrainWave.hpp>

#include <NaoFramework/Modules/ModuleInterface.hpp>
#include <NaoFramework/Log/Frontend.hpp>

namespace NaoFramework {
    namespace Core {
        BrainWave::BrainWave(std::string name) : Loggable(name, "BrainWave"), 
                                                 name_(name), running_(false) {}
        BrainWave::~BrainWave() {
            pause(); // We stop the thread when we die
        }

        BrainWave::BrainWave(BrainWave && other) : Loggable(std::move(other)),
                                                   name_(std::move(other.name_)), 
                                                   running_(other.running_.load(std::memory_order_acquire))
        {
            // If the other guy is running, we stop, copy data, and restart
            bool running = running_.load(std::memory_order_acquire);

            if ( running ) other.pause();

            modules_ = std::move(other.modules_);
            indices_ = std::move(other.indices_);

            if ( running ) execute();
        }
                                                           
        const BrainWave & BrainWave::operator=(BrainWave && other) {
            Loggable::operator=(std::move(other));

            pause(); // Stop whatever we where doing.

            name_ = std::move(other.name_); // It's important to remove the name so that we don't close its sink.

            bool running = running_ = other.running_.load(std::memory_order_acquire);

            if ( running ) other.pause();

            modules_ = std::move(other.modules_);
            indices_ = std::move(other.indices_);

            if ( running ) execute();

            return *this;
        }

        // Policy is no touchy at runtime (you can add them if you
        // reeeeally want but removing is definitely not supported
        // due to lots of additional work for little gain that
        // would need to going on in BBoard. All in all, limitations
        // are set by BBoard, this class limits itself to following 
        // those.
        void BrainWave::addModule(std::unique_ptr<Modules::ModuleInterface> && module) {
            bool running;
            if ( running = isRunning() ) pause();

            log( "Adding new module: " + module->getName() );
            // First we get the name
            indices_[module->getName()] = modules_.size()-1;
            // And at the end we move it away
            modules_.push_back(std::move(module));

            if ( running ) execute();
        }

        void BrainWave::launchWave() {
            log( "## Wave running.");
            while ( running_.load(std::memory_order_relaxed) ) {
                for ( auto & p : modules_ )
                    p->execute(); 
            }
            log( "## Wave quitting.");
        }

        void BrainWave::execute() {
            log( "Execute?");
            if ( running_.load(std::memory_order_acquire) ) return;
            log( "OK");

            running_.store(true, std::memory_order_release);

            // Run thread
            log( "Opening thread.");
            wave_ = std::thread(&BrainWave::launchWave, this);
            log( "Thread opened.");
        }

        void BrainWave::pause() {
            log( "Pause?");
            if ( !running_.load(std::memory_order_acquire) ) return;
            log( "OK");

            running_.store(false, std::memory_order_release);

            // Thread join
            log( "Joining");
            wave_.join();
            log( "Joined");
        }

        bool BrainWave::isRunning() const {
            return running_.load(std::memory_order_acquire);
        }

        const std::string & BrainWave::getName() const {
            return name_;
        }
    }
}
