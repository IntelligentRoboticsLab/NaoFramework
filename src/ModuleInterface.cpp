#include <NaoFramework/Modules/ModuleInterface.hpp>

namespace NaoFramework {
    namespace Modules {
        ModuleInterface::ModuleInterface(std::string moduleName) : name_(moduleName) {
            Log::makeSink(name_);
        }

        ModuleInterface::ModuleInterface(ModuleInterface&& other) : name_(other.name_) {
            other.name_ = "";
        }

        const ModuleInterface & ModuleInterface::operator=(ModuleInterface&& other) {
            name_ = other.name_;
            other.name_ = "";

            return *this;
        }

        ModuleInterface::~ModuleInterface() {
            Log::removeSink(name_); 
        }

        std::string ModuleInterface::getName() const {
            return name_;
        }

        void ModuleInterface::log(const std::string & text, Log::MessagePriority p) {
            Log::log(name_, text, p);
        }
    } // Modules
} //NaoFramework
