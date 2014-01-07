#include <NaoFramework/Modules/DynamicModule.hpp>

#include <stdexcept>

#include <dlfcn.h>

namespace NaoFramework {
    namespace Modules {
        // These macros are used to transform the names of the functions defined in 
        // DynamicModuleInterface.hpp into strings so we can load them.
        #define TO_STRING_2(X) #X
        #define TO_STRING(X) TO_STRING_2(X)
        #define FACTORY_NAME TO_STRING(NAO_FRAMEWORK_DYNAMIC_MODULE_FACTORY)
        #define DUMP_NAME    TO_STRING(NAO_FRAMEWORK_DYNAMIC_MODULE_DUMP)
        // We need a separate function because names are set in ModuleInterface:
        // we need to know the name of the module we are loading beforehand, but
        // we can't assume it from the library filename.
        std::unique_ptr<DynamicModule> makeDynamicModule(const std::string & moduleFilename,
                                                         Comm::LocalBlackboardAdapter & comm,
                                                         Comm::ExternalBlackboardAdapterMap & others)
        {
            // Load full library
            void * dllModule = dlopen(moduleFilename.c_str(), RTLD_GLOBAL | RTLD_NOW);
            if ( dllModule == nullptr ) throw std::runtime_error(dlerror());

            // Module maker
            dynamicModuleFactory* factory = (dynamicModuleFactory*) dlsym(dllModule, FACTORY_NAME);
            if ( factory == nullptr ) {
                dlclose(dllModule);    
                throw std::runtime_error(dlerror());
            }
            // We need to obtain this function now because we can't simply
            // create an instance we might not be able to delete!
            dynamicModuleDump* moduleDeleter = (dynamicModuleDump*) dlsym(dllModule, DUMP_NAME);
            if ( moduleDeleter == nullptr ) {
                dlclose(dllModule);    
                throw std::runtime_error(dlerror());
            }
            // This is managed by the DynamicModule and it is actually deleted within
            // the dll, so it's ok that we don't wrap the pointer up because we don't want
            // to actually delete it ourselves.
            try {
                DynamicModuleInterface* module = factory(comm, others);
                // One day we'll use make_unique...
                return std::unique_ptr<DynamicModule>( 
                            new DynamicModule("Dynamic" + module->getName(), dllModule, module, moduleDeleter) );
            } catch ( std::exception e ) {
                dlclose(dllModule);
                throw e;
            }
        }

        #undef FACTORY_NAME 
        #undef DUMP_NAME    
        #undef TO_STRING
        #undef TO_STRING_2

        DynamicModule::DynamicModule(std::string name, void * dllModule, DynamicModuleInterface * module, dynamicModuleDump * deleter) :
                                                                DynamicModuleInterface(name),
                                                                dllModule_(dllModule),
                                                                module_(module),
                                                                moduleDeleter_(deleter) {}


        DynamicModule::DynamicModule(DynamicModule && other) : 
                                                        DynamicModuleInterface(std::move(other)),
                                                        dllModule_(other.dllModule_),
                                                        module_(other.module_),
                                                        moduleDeleter_(other.moduleDeleter_)
        {
            other.dllModule_        = nullptr;
            other.module_           = nullptr;
        }

        const DynamicModule & DynamicModule::operator=(DynamicModule&& other) {
            DynamicModuleInterface::operator=(std::move(other));

            if (module_)    moduleDeleter_(&module_);
            if (dllModule_) dlclose(dllModule_);

            dllModule_              = other.dllModule_;
            module_                 = other.module_;
            moduleDeleter_          = other.moduleDeleter_;

            other.dllModule_        = nullptr;
            other.module_           = nullptr;

            return *this;
        }

        DynamicModule::~DynamicModule() {
            log("Deleting module..");
            if (module_)    moduleDeleter_(&module_);
            log("Dropping dll..");
            if (dllModule_) dlclose(dllModule_);
            log("Resources cleaned.");
        }

        void DynamicModule::execute() {
            module_->execute();
        }
    }
}
