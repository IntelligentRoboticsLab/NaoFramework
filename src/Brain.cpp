#include <NaoFramework/Core/Brain.hpp>
#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>
#include <NaoFramework/Comm/ExternalBlackboardAdapter.hpp>
#include <NaoFramework/Comm/ExternalBlackboardAdapterMap.hpp>
#include <NaoFramework/Comm/LocalBlackboardAdapter.hpp>

#include <iostream>

using std::cout;

namespace NaoFramework {
    namespace Core {
        class Brain::ExternalBlackboardMap : public Comm::ExternalBlackboardAdapterMap {
            public:
                ExternalBlackboardMap(Brain & b) : brain_(b) {}
                virtual Comm::ExternalBlackboardAdapter operator[]( const std::string & key ) {
                    if ( !brain_.waveExists(key) ) {
                        brain_.makeWave(key);
                        std::cout << "A new wave was referenced, and thus created: " << key << '\n';
                    }
                    return Comm::ExternalBlackboardAdapter(*(brain_.waves_.at(key).second));
                }
            private:
                Brain & brain_;
        };

        Brain::Brain() {}
        Brain::~Brain() {
            for ( auto & wave : waves_ )
                wave.second.first.pause();
            blackboards_.clear();
            waves_.clear();
        }

        bool Brain::waveExists(const std::string & wave) const {
            auto it = waves_.find(wave);
            return it != std::end(waves_);
        }

        void Brain::makeWave(const std::string & key) {
            blackboards_.emplace_front(key);
            waves_.emplace (key,
                            std::make_pair(
                                key,
                                blackboards_.begin()
                            ));
        }

        unsigned Brain::createWave(Inputs inputs) {
            if ( inputs.size() < 2 ) {
                std::cout << "Usage: " << inputs[0] << " wave_name\n";
                return 1;
            }

            std::cout << "Wave '" << inputs[1];
            if ( !waveExists(inputs[1]) ) {
                makeWave(inputs[1]);
                std::cout << "' created.\n"; 
            }
            else std::cout << "' exists already.\n";
            
            return 0;
        }

        unsigned Brain::addDynamicModule(Inputs inputs) {
            if ( inputs.size() < 3 ) {
                std::cout << "Usage: " << inputs[0] << " wave_name module_filename\n";
                return 1;
            }
            // Wave check
            if ( !waveExists(inputs[1]) ) {
                std::cout << "Error, wave '" << inputs[1] << "' does not exist.\n"; 
                return 1;
            }

            auto wave = inputs[1];
            auto module = inputs[2];

            unsigned loaded = 1; // 1 = Error!
            try {
                auto adapter   = Comm::LocalBlackboardAdapter(*(waves_.at(wave).second));
                auto globals   = ExternalBlackboardMap(*this);

                auto dynModule = Modules::makeDynamicModule(module, adapter, globals);

                auto moduleName = dynModule->getName();

                waves_.at(wave).first.addModule(std::move(dynModule)); // Give ownership -> dynModule empty

                std::cout << "Successfully loaded module: " << moduleName << "\n";
                loaded = 0;
            }
            catch ( std::runtime_error & e ) {
                std::cout << "Could not load module: " << e.what() << "\n";
            }
            return loaded;
        }

        unsigned Brain::execute(Inputs) {
            for ( auto & b : blackboards_ ) {
                if ( ! b.validateGlobals() ) {
                    std::cout << "Dependencies for wave '" << b.getName() << "' are not met!\n";
                    // We should give out better info on what is wrong. BBoard, save us!
                    return 1;
                }
            }
            // Launch threads
            for ( auto & wave : waves_ )
                wave.second.first.execute();

            std::cout << "All waves successfully started in the background!\n";

            return 0;
        }
    }
}
