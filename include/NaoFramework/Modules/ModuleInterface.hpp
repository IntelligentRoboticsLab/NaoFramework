#ifndef NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE

#include <NaoFramework/Log/Loggable.hpp>

#include <string>

namespace NaoFramework {
    namespace Modules {
        /**
         * @brief This class represents the interface for every module supported by the framework.
         *
         * Each module provides a single entry point, execute(), which is called by the framework
         * in order to allow the module to carry out its work. Each module has a name, and in 
         * general this framework disallows modules with the same name. Thus, copying a module 
         * is disallowsed.
         */
        class ModuleInterface : public Log::Loggable {
            public:
                /**
                 * @brief Basic constructor.
                 *
                 * @param moduleName The name of the module.
                 */
                ModuleInterface(std::string moduleName);
                // No module copying
                ModuleInterface(const ModuleInterface&) = delete;
                const ModuleInterface & operator=(const ModuleInterface&) = delete;
                // Yay module moving
                ModuleInterface(ModuleInterface &&);
                const ModuleInterface & operator=(ModuleInterface &&);

                /**
                 * @brief Virtual destructor.
                 *
                 * We provide a virtual destructor in order to be able to call module-specific
                 * cleanup code.
                 */
                virtual ~ModuleInterface();

                /**
                 * @brief This function is the interface for the entry point of the module.
                 *
                 * All work performed by a module resides in the implementation of this function.
                 */
                virtual void execute() = 0;

                /**
                 * @brief This function retrieves the module's name.
                 *
                 * @return The module's name.
                 */
                const std::string & getName() const;

            protected:
                std::string name_;
        };
    } // Modules
} //NaoFramework

#endif
