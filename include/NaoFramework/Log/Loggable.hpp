#ifndef NAO_FRAMEWORK_LOG_LOGGABLE_HEADER_FILE
#define NAO_FRAMEWORK_LOG_LOGGABLE_HEADER_FILE

#include <NaoFramework/Log/Types.hpp>

#include <string>

namespace NaoFramework {
    namespace Log {
        class Loggable {
            public:
                Loggable(std::string name, std::string folder);
                ~Loggable();

                Loggable(const Loggable&) = delete;
                const Loggable& operator=(const Loggable&) = delete;

                Loggable(Loggable&&);
                const Loggable& operator=(Loggable&&);

                void log(const std::string & message, MessagePriority priority = Info);
            private:
                std::string name_;
                std::string folder_;
        };
    }
}

#endif
