#ifndef NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE

#include <boost/any.hpp>

namespace NaoFramework {
    namespace Modules {
        class ModuleInterface {
            public:
                ModuleInterface(boost::any * comm) : comm_(comm) {}
                virtual ~ModuleInterface() {}
                virtual void print() = 0;
            protected:
                boost::any * comm_;
        };
    } // Modules
} //NaoFramework

#endif
