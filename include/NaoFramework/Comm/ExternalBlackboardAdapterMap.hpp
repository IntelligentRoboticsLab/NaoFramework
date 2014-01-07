#ifndef NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_MAP_HEADER_FILE
#define NAO_FRAMEWORK_COMM_EXTERNAL_BLACKBOARD_ADAPTER_MAP_HEADER_FILE

#include <string>

namespace NaoFramework {
    namespace Comm {
        class ExternalBlackboardAdapter;

        /**
         * @brief This class is an interface for external access to multiple Blackboards.
         *
         * This class represents an interface and does not contain any implementation details
         * because the way it needs to be implemented is deeply coupled with the way the 
         * framework actually stores and accesses individual Blackboards.
         */
        class ExternalBlackboardAdapterMap {
            public:
                /**
                 * @brief This operator allows map-like, external access to many Blackboards.
                 *
                 * The idea here is that modules access this. If the element does not yet exist
                 * it is created in the underlying container. Otherwise it is accessed.
                 *
                 * @param key The name of the Blackboard/BrainWave that the module wants to access.
                 *
                 * @return An ExternalBlackboardAdapter to the required Blackboard.
                 */
                virtual ExternalBlackboardAdapter operator[]( const std::string & key ) = 0;
        };
    }
}

#endif
