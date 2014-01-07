#ifndef NAO_FRAMEWORK_LOG_LOGGER_HEADER_FILE
#define NAO_FRAMEWORK_LOG_LOGGER_HEADER_FILE

#include <NaoFramework/Log/Types.hpp>

#include <string>

namespace NaoFramework {
    namespace Log {
        /**
         * @brief This function initializes the log backend. It should always be called at startup.
         *
         * @param folder This names the folder that will hold all logged records.
         */
        void init       (const std::string & folder = "log");

        /**
         * @brief This function creates a new sink.
         *
         * This function creates a new sink for a single client in the specified subfolder. The file
         * containing logs will be named as the client. Clients are registered and deregistered on a 
         * name-folder way, so that identically named clients will not collide as long as their
         * folder is different.
         *
         * @param client
         * @param subfolder
         *
         * @return 
         */
        bool makeSink   (const std::string & client, const std::string & subfolder);
        void removeSink (const std::string & client, const std::string & subfolder);

        void log        (const std::string & client, 
                         const std::string & subfolder, 
                         const std::string & message, 
                         MessagePriority priority = Info);
    }
}

#endif
