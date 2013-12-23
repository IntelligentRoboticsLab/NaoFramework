#include <NaoFramework/Comm/Blackboard.hpp>

namespace NaoFramework {
    namespace Comm {
        Blackboard::Blackboard(std::string name) : name_(name) { Log::makeSink(name_, "Blackboard"); }
        Blackboard::~Blackboard() { Log::removeSink(name_); }

        bool Blackboard::validateGlobals() const {
            for ( auto pair : typeCheck_ ) {
                if ( std::get<0>(pair.second) == TypeState::Requested ) return false;
            }
            return true;
        }
    }
}
