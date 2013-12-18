#ifndef NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE

#include <NaoFramework/Modules/DynamicModuleInterface.hpp>
#include <string>

namespace NaoFramework {
    namespace Modules {
        class DynamicModule;

        DynamicModule makeDynamicModule(std::string moduleFilename, boost::any * comm);

        class DynamicModule : public DynamicModuleInterface {
            public:
                // We refine ModuleInterface move constructor
                DynamicModule(DynamicModule&& module);
                const DynamicModule & operator=(DynamicModule&& module);

                virtual ~DynamicModule();
                
                // Actual module things
                virtual void print();
            private:
                void * dllModule_;
                DynamicModuleInterface * module_;   // This is a class
                dynamicModuleDump * moduleDeleter_; // This is a function

                DynamicModule(std::string name, boost::any * comm, void * dllModule, DynamicModuleInterface * module, dynamicModuleDump * deleter);
                friend DynamicModule makeDynamicModule(std::string moduleFilename, boost::any * comm);
        };
    } // Modules
} //NaoFramework

#endif
