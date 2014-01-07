#include <NaoFramework/Modules/ModuleInterface.hpp>

namespace NaoFramework {
    namespace Modules {
        ModuleInterface::ModuleInterface(std::string moduleName) : Loggable(moduleName, "Modules"), name_(moduleName) {}
        ModuleInterface::~ModuleInterface() {}

        ModuleInterface::ModuleInterface(ModuleInterface&& other) : Loggable(std::move(other)),  name_(std::move(other.name_)) {}

        const ModuleInterface & ModuleInterface::operator=(ModuleInterface&& other) {
            Loggable::operator=(std::move(other));

            name_ = std::move(other.name_);

            return *this;
        }

        const std::string & ModuleInterface::getName() const {
            return name_;
        }
    } // Modules
} //NaoFramework
