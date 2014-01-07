#ifndef NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_INTERFACE_HEADER_FILE

#include <NaoFramework/Comm/LocalBlackboardAdapter.hpp>
#include <NaoFramework/Comm/ExternalBlackboardAdapterMap.hpp>

#include <NaoFramework/Modules/ModuleInterface.hpp>

namespace NaoFramework {
    namespace Modules {
        /**
         * @brief This class represents the interface for modules loaded as shared libraries.
         *
         * This class is the interface for dynamically loaded modules. Each module loaded 
         * dynamically needs to both inherit from this class, and to call the MODULE_EXPORT
         * macro defined below AFTER the class definition (or provide instead equivalent 
         * construction/deconstruction facilities). This file also includes automatically
         * the definitions of the communication facilities, so that each module can keep
         * its own includes to a minimum.
         *
         * It is important to notice that while this class needs just a name to be constructed,
         * the construction facilities created through the MODULE_EXPORT macro require 
         * any child module's constructor to take references to both a LocalBlackboardAdapter
         * and to an ExternalBlackboardAdapterMap. This is so that a module can register 
         * its own provide and requires to the framework. Such references must not be saved
         * as the objects they reference are temporaries and will likely be destructed just 
         * after the construction of the module.
         */
        class DynamicModuleInterface : public ModuleInterface {
            public:
                using ModuleInterface::ModuleInterface;
        };

        using dynamicModuleFactory  = DynamicModuleInterface* (Comm::LocalBlackboardAdapter &, Comm::ExternalBlackboardAdapterMap &);
        using dynamicModuleDump     = void                    (DynamicModuleInterface**);

    } // Modules
} //NaoFramework

#define NAO_FRAMEWORK_DYNAMIC_MODULE_FACTORY naoFrameworkDynamicModuleFactory
#define NAO_FRAMEWORK_DYNAMIC_MODULE_DUMP    naoFrameworkDynamicModuleDump

// We need a deleter because the module 
// may redefine operator delete.

/**
 * @brief This macro creates importable constructors/deconstructors for a module.
 *
 * Note that the module's constructor will need to take different parameters than their parents!
 *
 * @param X The name of the module class.
 */
#define MODULE_EXPORT(X)                                                                                                \
extern "C" {                                                                                                            \
    NaoFramework::Modules::dynamicModuleFactory NAO_FRAMEWORK_DYNAMIC_MODULE_FACTORY;                                   \
    NaoFramework::Modules::dynamicModuleDump    NAO_FRAMEWORK_DYNAMIC_MODULE_DUMP;                                      \
                                                                                                                        \
    NaoFramework::Modules::DynamicModuleInterface* NAO_FRAMEWORK_DYNAMIC_MODULE_FACTORY                                 \
    ( NaoFramework::Comm::LocalBlackboardAdapter & comm, NaoFramework::Comm::ExternalBlackboardAdapterMap & others ) {  \
        return new X(comm, others);                                                                                     \
    }                                                                                                                   \
    void NAO_FRAMEWORK_DYNAMIC_MODULE_DUMP(NaoFramework::Modules::DynamicModuleInterface**x) {                          \
        delete *x;                                                                                                      \
        *x = nullptr;                                                                                                   \
    }                                                                                                                   \
}

#endif // Header guard
