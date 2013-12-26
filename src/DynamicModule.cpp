#include <NaoFramework/Modules/DynamicModule.hpp>

#include <stdexcept>

#include <dlfcn.h>

namespace NaoFramework {
    namespace Modules {
        #define TO_STRING_2(X) #X
        #define TO_STRING(X) TO_STRING_2(X)
        #define FACTORY_NAME TO_STRING(NAO_FRAMEWORK_DYNAMIC_MODULE_FACTORY)
        #define DUMP_NAME    TO_STRING(NAO_FRAMEWORK_DYNAMIC_MODULE_DUMP)
        // We need a separate function because of names passed to ModuleInterface:
        // we need to know the name of the module we are loading beforehand, but
        // we can't assume it from the library name.
        std::unique_ptr<DynamicModule> makeDynamicModule(const std::string & moduleFilename,
                                                         Comm::LocalBlackboardAdapter & comm,
                                                         Comm::ExternalBlackboardAdapterMap & others)
        {
            // Load full library
            void * dllModule = dlopen(moduleFilename.c_str(), RTLD_GLOBAL | RTLD_NOW);
            if ( dllModule == nullptr ) throw std::runtime_error(dlerror());

            // This we don't need to save..
            dynamicModuleFactory* factory = (dynamicModuleFactory*) dlsym(dllModule, FACTORY_NAME);
            if ( factory == nullptr ) throw std::runtime_error(dlerror());
            // We need to obtain this function now because we can't simply
            // create an instance we might not be able to delete!
            dynamicModuleDump* moduleDeleter = (dynamicModuleDump*) dlsym(dllModule, DUMP_NAME );
            if ( moduleDeleter == nullptr ) throw std::runtime_error(dlerror());
            // This is managed by the DynamicModule and it is actually deleted within
            // the dll itself, so it's ok that we don't wrap it up because we don't want
            // to actually delete it.
            DynamicModuleInterface* module = factory(comm, others);
            // One day we'll use make_unique...
            return std::unique_ptr<DynamicModule>( new DynamicModule("Dynamic" + module->getName(), dllModule, module, moduleDeleter));
        }

        #undef FACTORY_NAME 
        #undef DUMP_NAME    
        #undef TO_STRING

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
