#ifndef NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE
#define NAO_FRAMEWORK_MODULES_DYNAMIC_MODULE_HEADER_FILE

#include <NaoFramework/Modules/DynamicModuleInterface.hpp>

#include <string>
#include <memory>

namespace NaoFramework {
    namespace Modules {
        class DynamicModule;

        /**
         * @brief This function loads a module from a dynamic library and wraps it into a DynamicModule.
         *
         * This function has the responsibility of talking to the module, verifying that required 
         * facilities are there, and if everything goes correctly it returns a dynamically allocated
         * wrapper for the module. Pointers are used so that we can leverage virtual functions and
         * store ModuleInterfaces and still do the right things.
         *
         * This function is also kept separate from the DynamicModule wrapper class so that we can 
         * read the module's name before having to assign one to the wrapper (which gets set to a 
         * variation of the loaded module's name to keep logs separated).
         *
         * @param moduleFilename The name of the shared library containing the module.
         * @param mainComm The adapter for the module's BrainWave.
         * @param externalComm The adapter for all other BrainWaves.
         *
         * @return A pointer to the wrapper of the loaded module.
         * @throws If anything goes wrong, this function will throw an std::runtime_error.
         */
        std::unique_ptr<DynamicModule> makeDynamicModule(const std::string & moduleFilename, Comm::LocalBlackboardAdapter & mainComm, Comm::ExternalBlackboardAdapterMap & externalComm);

        /**
         * @brief This class represents a wrapper for a module loaded as shared library.
         *
         * This class inherits from DynamicModuleInterface so that it can keep its interface
         * consistent with the one adopted by the modules it manages, and simply reroute 
         * every appropriate method to the one provided by the shared library.
         */
        class DynamicModule : public DynamicModuleInterface {
            public:
                // We refine ModuleInterface move constructor
                DynamicModule(DynamicModule&& module);
                const DynamicModule & operator=(DynamicModule&& module);

                /**
                 * @brief Virtual destructor.
                 *
                 * This destructor deletes the underlying module, and reduces
                 * the counter of usage of the underlying shared library by one
                 * by calling dlclose().
                 */
                virtual ~DynamicModule();
                
                /**
                 * @brief This function rerouts callers to the execute() method of the wrapped module.
                 */
                virtual void execute();
            private:
                void * dllModule_;
                DynamicModuleInterface * module_;   // This is a class
                dynamicModuleDump * moduleDeleter_; // This is a function

                /**
                 * @brief Basic constructor.
                 *
                 * @param name The name of the wrapper.
                 * @param dllModule A pointer to the shared library memory location.
                 * @param module A pointer to an allocated instance of the loaded module.
                 * @param deleter A pointer to the destructor function provided by the module.
                 */
                DynamicModule(std::string name, void * dllModule, DynamicModuleInterface * module, dynamicModuleDump * deleter);
                friend std::unique_ptr<DynamicModule> makeDynamicModule(const std::string &, Comm::LocalBlackboardAdapter &, Comm::ExternalBlackboardAdapterMap &);
        };
    } // Modules
} //NaoFramework

#endif
