#ifndef NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_MODULE_INTERFACE_HEADER_FILE

#include <string>
// We don't forward declare these mostly because then we can force
// external modules to have them and check what they are doing.
#include <boost/any.hpp>
// Normally we would use std::shared_ptr
// But this library does not accept it
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

namespace NaoFramework {
    namespace Modules {
        class ModuleInterface {
            public:
                ModuleInterface(std::string moduleName, boost::any * comm);
                // No module copying
                ModuleInterface(const ModuleInterface&) = delete;
                const ModuleInterface & operator=(const ModuleInterface&) = delete;
                // Yay module moving
                ModuleInterface(ModuleInterface &&);
                const ModuleInterface & operator=(ModuleInterface &&);

                virtual ~ModuleInterface();
                virtual void print() = 0;
                
                std::string getName() const;

                void log(const std::string &);

            protected:
                std::string name_;
                boost::any * comm_;

            private:
                using TextSink = boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>;
                boost::shared_ptr<TextSink> sink_;
        };
    } // Modules
} //NaoFramework

#endif
