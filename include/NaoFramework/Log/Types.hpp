#ifndef NAO_FRAMEWORK_LOG_TYPES_HEADER_FILE
#define NAO_FRAMEWORK_LOG_TYPES_HEADER_FILE

namespace NaoFramework {
    namespace Log {
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
    }
}

#endif
