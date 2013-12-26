#include <NaoFramework/Modules/ModuleInterface.hpp>

namespace NaoFramework {
    namespace Modules {
        ModuleInterface::ModuleInterface(std::string moduleName) : name_(moduleName) {
            Log::makeSink(name_, "Modules");
        }

        ModuleInterface::~ModuleInterface() {
            Log::removeSink(name_); 
        }

        ModuleInterface::ModuleInterface(ModuleInterface&& other) : name_(std::move(other.name_)) {}

        const ModuleInterface & ModuleInterface::operator=(ModuleInterface&& other) {
            Log::removeSink(name_); 

            name_ = std::move(other.name_);

            return *this;
        }

        const std::string & ModuleInterface::getName() const {
            return name_;
        }

        void ModuleInterface::log(const std::string & text, Log::MessagePriority p) {
            Log::log(name_, text, p);
        }
    } // Modules
} //NaoFramework
