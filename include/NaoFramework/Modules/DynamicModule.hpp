#ifndef NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE

#include <NaoFramework/Modules/DynamicModuleInterface.hpp>

#include <string>
#include <memory>

namespace NaoFramework {
    namespace Modules {
        class DynamicModule;

        std::unique_ptr<DynamicModule> makeDynamicModule(const std::string & moduleFilename, Comm::LocalBlackboardAdapter & mainComm, Comm::ExternalBlackboardAdapterMap & externalComm);

        class DynamicModule : public DynamicModuleInterface {
            public:
                // We refine ModuleInterface move constructor
                DynamicModule(DynamicModule&& module);
                const DynamicModule & operator=(DynamicModule&& module);

                virtual ~DynamicModule();
                
                // Actual module things
                virtual void execute();
            private:
                void * dllModule_;
                DynamicModuleInterface * module_;   // This is a class
                dynamicModuleDump * moduleDeleter_; // This is a function

                DynamicModule(std::string name, void * dllModule, DynamicModuleInterface * module, dynamicModuleDump * deleter);
                friend std::unique_ptr<DynamicModule> makeDynamicModule(const std::string &, Comm::LocalBlackboardAdapter &, Comm::ExternalBlackboardAdapterMap &);
        };
    } // Modules
} //NaoFramework

#endif
