#ifndef NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE

#include <NaoFramework/Log/Frontend.hpp>

#include <string>

namespace NaoFramework {
    namespace Modules {
        class ModuleInterface {
            public:
                ModuleInterface(std::string moduleName);
                // No module copying
                ModuleInterface(const ModuleInterface&) = delete;
                const ModuleInterface & operator=(const ModuleInterface&) = delete;
                // Yay module moving
                ModuleInterface(ModuleInterface &&);
                const ModuleInterface & operator=(ModuleInterface &&);

                virtual ~ModuleInterface();
                virtual void execute() = 0;

                std::string getName() const;

                void log(const std::string &, Log::MessagePriority p = Log::Info);

            protected:
                std::string name_;
        };
    } // Modules
} //NaoFramework

#endif
