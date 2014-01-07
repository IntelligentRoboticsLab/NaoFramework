#include <NaoFramework/Log/Loggable.hpp>

#include <NaoFramework/Log/Frontend.hpp>

namespace NaoFramework {
    namespace Log {
        Loggable::Loggable(std::string name, std::string folder) : name_(name), folder_(folder) {
            makeSink(name_, folder_);
        }
        Loggable::~Loggable() {
            removeSink(name_, folder_);
        }

        Loggable::Loggable(Loggable&& other) : name_(std::move(other.name_)), folder_(std::move(other.folder_)) {}

        const Loggable& Loggable::operator=(Loggable&& other) {
            removeSink(name_, folder_);

            name_ = std::move(other.name_);
            folder_ = std::move(other.folder_);
        }

        void Loggable::log(const std::string & message, MessagePriority priority) {
            NaoFramework::Log::log(name_, folder_, message, priority);
        }
    }
}
