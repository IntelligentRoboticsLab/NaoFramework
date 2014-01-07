#include <NaoFramework/Comm/Blackboard.hpp>

namespace NaoFramework {
    namespace Comm {
        Blackboard::Blackboard(std::string name) : Loggable(name, "Blackboard"), name_(name) {}
        Blackboard::~Blackboard() {}

        bool Blackboard::validateGlobals() const {
            for ( auto pair : typeCheck_ ) {
                if ( std::get<0>(pair.second) == TypeState::Requested ) return false;
            }
            return true;
        }

        const std::string & Blackboard::getName() const {
            return name_;
        }
    }
}
