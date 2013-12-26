#ifndef NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_HEADER_FILE
#define NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_HEADER_FILE

#include <NaoFramework/Comm/Types.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>

#include <functional>

namespace NaoFramework {
    namespace Comm {
        class ExternalBlackboardAdapter {
            public:
                ExternalBlackboardAdapter(Blackboard & b) : blackboard_(b) {}

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
