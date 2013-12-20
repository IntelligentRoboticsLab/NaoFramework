#ifndef NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE

#include <string>
// We don't forward declare these mostly because then we can force
// external modules to have them and check what they are doing.
#include <boost/any.hpp>
#include <NaoFramework/Log/Frontend.hpp>

namespace NaoFramework {
    namespace Modules {
        class ModuleInterface {
            public:
                ModuleInterface(std::string moduleName, boost::any * comm);
                // No module copying
                ModuleInterface(const ModuleInterface&) = delete;
                const ModuleInterface & operator=(const ModuleInterface&) = delete;
                // Yay module moving
                ModuleInterface(ModuleInterface &&);
                const ModuleInterface & operator=(ModuleInterface &&);

                virtual ~ModuleInterface();
                virtual void print() = 0;

                std::string getName() const;

                void log(const std::string &, Log::MessagePriority p = Log::Info);

            protected:
                std::string name_;
                boost::any * comm_;
        };
    } // Modules
} //NaoFramework

#endif
