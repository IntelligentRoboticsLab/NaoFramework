#include <NaoFramework/Modules/DynamicModule.hpp>

#include <boost/any.hpp>
#include <stdexcept>
#include <dlfcn.h>

#include <iostream>

using std::cout;
namespace NaoFramework {
    namespace Modules {
        #define TO_STRING_2(X) #X
        #define TO_STRING(X) TO_STRING_2(X)
        #define FACTORY_NAME TO_STRING(NAO_FRAMEWORK_DYNAMIC_MODULE_FACTORY)
        #define DUMP_NAME    TO_STRING(NAO_FRAMEWORK_DYNAMIC_MODULE_DUMP)

        DynamicModule::DynamicModule(std::string moduleName, std::string moduleFilename, boost::any * comm) :
                                                                DynamicModuleInterface(moduleName, comm),
                                                                dllModule_(nullptr),
                                                                module_(nullptr),
                                                                moduleDeleter_(nullptr)
        {
            // Load full library
            dllModule_ = dlopen(moduleFilename.c_str(), RTLD_GLOBAL | RTLD_NOW);
            if ( dllModule_ == nullptr ) throw std::runtime_error(dlerror());

            // This we don't need to save..
            dynamicModuleFactory* factory = (dynamicModuleFactory*) dlsym(dllModule_, FACTORY_NAME);
            if ( factory == nullptr ) throw std::runtime_error(dlerror());
            // We need to obtain this function now because we can't simply
            // create an instance we might not be able to delete!
            moduleDeleter_ = (dynamicModuleDump*) dlsym(dllModule_, DUMP_NAME );
            if ( moduleDeleter_ == nullptr ) throw std::runtime_error(dlerror());

            module_ = factory(comm_);
        }

        DynamicModule::DynamicModule(DynamicModule && other) : 
                                                        DynamicModuleInterface(other.name_, other.comm_),
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
            comm_                   = other.comm_;

            other.dllModule_        = nullptr;
            other.module_           = nullptr;

            return *this;
        }

        DynamicModule::~DynamicModule() {
            if (module_)    moduleDeleter_(&module_);
            if (dllModule_) dlclose(dllModule_);
        }

        void DynamicModule::print() {
            module_->print();
        }

        #undef FACTORY_NAME 
        #undef DUMP_NAME    
        #undef TO_STRING
    }
}
