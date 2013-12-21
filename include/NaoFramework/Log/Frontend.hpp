#ifndef NAO_FRAMEWORK_LOG_LOGGER_HEADER_FILE
#define NAO_FRAMEWORK_LOG_LOGGER_HEADER_FILE

#include <string>

namespace NaoFramework {
    namespace Log {
        void init       (const std::string & logFolder = "log/");

        bool makeSink   (const std::string & client);
        void removeSink (const std::string & client);

        // Do not update this as it mirrors
        // a boost struct, so we can avoid including 500 headers for no reason at all.
        // In case something goes wrong, this is defined in boost/log/trivial.hpp so you can check it.
        enum MessagePriority {
            Trace,
            Debug,
            Info,
            Warning,
            Error,
            Fatal
        };

        void log        (const std::string & client, const std::string & message, MessagePriority priority = Info);
    }
}

#endif
