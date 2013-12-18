#ifndef NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE

#include <string>
#include <boost/any.hpp>

namespace NaoFramework {
    namespace Modules {
        class ModuleInterface {
            public:
                ModuleInterface(std::string moduleName, boost::any * comm) : name_(moduleName), comm_(comm) {}
                virtual ~ModuleInterface() {}
                virtual void print() = 0;
                
                std::string getName() const { return name_; }
            protected:
                std::string name_;
                boost::any * comm_;
        };
    } // Modules
} //NaoFramework

#endif
