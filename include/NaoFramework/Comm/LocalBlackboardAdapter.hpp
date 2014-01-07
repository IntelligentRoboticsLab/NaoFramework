#ifndef NAO_FRAMEWORK_COMM_LOCAL_BLACKBOARD_ADAPTER_HEADER_FILE
#define NAO_FRAMEWORK_COMM_LOCAL_BLACKBOARD_ADAPTER_HEADER_FILE

#include <NaoFramework/Comm/Types.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>

#include <functional>

namespace NaoFramework {
    namespace Comm {
        /**
         * @brief This class provides a temporary wrapper around Blackboard for limited access.
         *
         * This adapter is provided as a means to shield/block modules registering requests to their
         * local Blackboard from using non-local functions. All functions are simply redirects to
         * their equivalent functions within Blackboard.
         */
        class LocalBlackboardAdapter {
            public:
                /**
                 * @brief Basic constructor.
                 *
                 * @param b A reference to a Blackboard instance.
                 */
                LocalBlackboardAdapter(Blackboard & b) : blackboard_(b) {} 

                /// \sa Blackboard::registerRequire()
                template <class T>
                RequireFunction<T> registerRequire          (const std::string & s, RegistrationError * e = nullptr) {
                    return blackboard_.registerRequire<T>(s,e);
                }

                /// \sa Blackboard::registerProvide()
                template <class T>
                ProvideFunction<T> registerProvide          (const std::string & s, RegistrationError * e = nullptr) {
                    return blackboard_.registerProvide<T>(s,e);
                }

                /// \sa Blackboard::registerGlobalProvide()
                template <class T>
                ProvideFunction<T> registerGlobalProvide    (const std::string & s, const T & v, RegistrationError * e = nullptr) {
                    return blackboard_.registerGlobalProvide<T>(s, v, e);
                }
            private:
                Blackboard & blackboard_;
        };
    }
}

#endif
