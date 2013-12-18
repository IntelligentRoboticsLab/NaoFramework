#ifndef NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE

#include <NaoFramework/Modules/DynamicModuleInterface.hpp>
#include <string>

namespace NaoFramework {
    namespace Modules {
        class DynamicModule : public DynamicModuleInterface {
            public:
                DynamicModule(std::string moduleName, std::string moduleFilename, boost::any * comm);

                DynamicModule(DynamicModule&& module);
                const DynamicModule & operator=(DynamicModule&& module);
                // Can't copy this, sorry!
                DynamicModule(const DynamicModule& module) = delete;
                const DynamicModule & operator=(const DynamicModule&) = delete;

                virtual ~DynamicModule();
                
                // Actual module things
                virtual void print();
            private:
                void * dllModule_;
                DynamicModuleInterface * module_;
                dynamicModuleDump * moduleDeleter_;
        };
    } // Modules
} //NaoFramework

#endif
