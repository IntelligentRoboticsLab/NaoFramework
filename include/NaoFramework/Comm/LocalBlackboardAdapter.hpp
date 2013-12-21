#ifndef NAO_FRAMEWORK_COMM_LOCAL_BLACKBOARD_ADAPTER
#define NAO_FRAMEWORK_COMM_LOCAL_BLACKBOARD_ADAPTER

#include <NaoFramework/Comm/Types.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>

#include <functional>

namespace NaoFramework {
    namespace Comm {
        class LocalBlackboardAdapter {
            public:
                LocalBlackboardAdapter(Blackboard & b) : blackboard_(b) {} 

                template <class T>
                RequireFunction<T> registerRequire          (const std::string & s, RegistrationError * e = nullptr) {
                    return blackboard_.registerRequire<T>(s,e);
                }

                // The function returned here takes a T value and overwrites the appropriate key
                // with it.
                template <class T>
                ProvideFunction<T> registerProvide          (const std::string & s, RegistrationError * e = nullptr) {
                    return blackboard_.registerProvide<T>(s,e);
                }

                // This function not only registers a provide, but sets it, so that inter-thread
                // requires don't break no matter the thread-order. Info that has to be read outside
                // of the thread where it is provided should use this function.
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
