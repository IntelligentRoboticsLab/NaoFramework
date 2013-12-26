#ifndef NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_MAP_HEADER_FILE
#define NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_MAP_HEADER_FILE

#include <string>

namespace NaoFramework {
    namespace Comm {
        class ExternalBlackboardAdapter;

        class ExternalBlackboardAdapterMap {
            public:
                // The idea here is that modules access this. If the element does not yet exist
                // it is created in the underlying map. Otherwise it is accessed.
                virtual ExternalBlackboardAdapter operator[]( const std::string & key ) = 0;
        };
    }
}

#endif
