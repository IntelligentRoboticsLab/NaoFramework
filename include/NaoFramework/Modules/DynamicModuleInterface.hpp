#ifndef NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_INTERFACE_HEADER_FILE

#include <NaoFramework/Comm/LocalBlackboardAdapter.hpp>
#include <NaoFramework/Comm/ExternalBlackboardAdapterMap.hpp>

#include <NaoFramework/Modules/ModuleInterface.hpp>

namespace NaoFramework {
    namespace Modules {
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
