#ifndef NAO_FRAMEWORK_COMM_TYPES_HEADER_FILE
#define NAO_FRAMEWORK_COMM_TYPES_HEADER_FILE

#include <functional>

namespace NaoFramework {
    namespace Comm {
        enum class RegistrationError {
            None,
            Requested,
            LocallyProvided,
            GloballyProvided,
            WrongType
        };
        template <class T>
        using ProvideFunction   = std::function<void(const T&)>;
        template <class T>
        using RequireFunction   = std::function<T()>;
    }
}

#endif
