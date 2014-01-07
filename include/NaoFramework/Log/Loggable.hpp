#ifndef NAO_FRAMEWORK_LOG_LOGGABLE_HEADER_FILE
#define NAO_FRAMEWORK_LOG_LOGGABLE_HEADER_FILE

#include <NaoFramework/Log/Types.hpp>

#include <string>

namespace NaoFramework {
    namespace Log {
        /**
         * @brief This class provides an easy way to log information.
         *
         * This class automatically interfaces with the log frontend functions, 
         * which in turn talk directly with the library used to implement logging.
         *
         * The function log can be used directly to log information to the file that
         * has been specified during the object's construction.
         *
         * Please keep in mind that for children that provide move semantics, they have
         * to call explicitly move constructor/assignment of this class, otherwise
         * resources won't get moved.
         */
        class Loggable {
            public:
                /**
                 * @brief Basic constructor, sets up a new sink.
                 *
                 * This constructor creates a new sink, situated in folder 'folder' 
                 * and in file 'name' within the main logging folder setup during
                 * log initialization (in the log frontend file).
                 * 
                 * A name-folder pair should be unique, as the underlying implementation
                 * does not keep reference count of the number of writers to a certain
                 * sink and the sink will get closed as soon as any of the Loggable objects
                 * pointing to it gets deleted.
                 *
                 * @param name The name of the file to use to log.
                 * @param folder The name of the folder to use to log.
                 */
                Loggable(std::string name, std::string folder);

                /**
                 * @brief Besic destructor, removes the sink.
                 */
                ~Loggable();

                Loggable(const Loggable&) = delete;
                const Loggable& operator=(const Loggable&) = delete;

                /**
                 * @brief Move constructor, transfers ownership of the sink.
                 *
                 * @param other The moved object.
                 */
                Loggable(Loggable&& other);

                /**
                 * @brief Move assignment operator, transfers ownership of a sink and closes the old one.
                 *
                 * @param other The moved object.
                 *
                 * @return The Loggable object itself.
                 */
                const Loggable& operator=(Loggable&& other);

                /**
                 * @brief This function logs a message to the sink associated with this object.
                 *
                 * @param message The message to be logged.
                 * @param priority The priority of the message.
                 */
                void log(const std::string & message, MessagePriority priority = Info);
            private:
                std::string name_;
                std::string folder_;
        };
    }
}

#endif
