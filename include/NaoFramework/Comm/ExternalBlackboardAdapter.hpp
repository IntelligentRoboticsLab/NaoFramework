#ifndef NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_HEADER_FILE
#define NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_HEADER_FILE

#include <NaoFramework/Comm/Types.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>

#include <functional>

namespace NaoFramework {
    namespace Comm {
        /**
         * @brief This class provides a temporary wrapper around Blackboard for limited access.
         *
         * This adapter is provided as a means to shield/block modules registering requests to 
         * Blackboards of other threads from using local functions. All functions are simply 
         * redirects to their equivalent functions within Blackboard.
         */
        class ExternalBlackboardAdapter {
            public:
                /**
                 * @brief Basic constructor.
                 *
                 * @param b A reference to a Blackboard instance.
                 */
                ExternalBlackboardAdapter(Blackboard & b) : blackboard_(b) {}

                /// \sa Blackboard::registerGlobalRequire()
                template <class T>
                RequireFunction<T> registerGlobalRequire    (const std::string & s, RegistrationError * e = nullptr) {
                    return blackboard_.registerGlobalRequire<T>(s,e);
                }
            private:
                Blackboard & blackboard_;
        };
    }
}

#endif
