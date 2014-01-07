#ifndef NAO_FRAMEWORK_COMM_BLACKBOARD_HEADER_FILE
#define NAO_FRAMEWORK_COMM_BLACKBOARD_HEADER_FILE

#include <NaoFramework/Comm/Types.hpp>
#include <NaoFramework/Log/Loggable.hpp>

#include <unordered_map>
#include <functional>
#include <typeindex>

#include <boost/any.hpp>
#include <boost/thread.hpp>

namespace NaoFramework {
    namespace Comm {
        /**
         * @brief This class provides communication facilities between modules.
         *
         * This class implements in-out communication for a single thread, and out-only
         * communication from a thread to many threads. The object allows registration of data
         * requests and data provisions of any size and type. 
         *                                                                                         
         * Blackboard is able to perform run-time type checking. Pre-emptive checks are performed
         * for single threaded requests, so that invalid data requests/provisions are treated
         * as errors. It is important to notice that such errors do NOT throw, but instead are
         * left to the caller modules to manage (and they can throw if they want to).
         *
         * In addition Blackboard provides a method called validateGlobals() that checks 
         * for interdependent and cyclic thread data dependencies, but only once all requests 
         * have been registered. This method has to be called manually.
         * 
         * Thread local communication works as follows. Each module must register its own 
         * requirements for data and its own provisions of data. Each registration must follow
         * the order of module execution within the local thread, as typechecking
         * happens in a sequential manner.
         *
         * A module can register a request for data with a certain key and type.
         * The request is accepted unless:
         *
         * - The specified key is being provided with a different type at the time of the request.
         * - The specified key is not being provided at all, but a different request with a 
         *   different type has already been accepted for this key.
         *
         * \sa registerRequire()
         *
         * A module can register a provision of data with a certain key and type.
         * The request is accepted unless:
         *
         * - The specified key has not yet been provided, but it has been requested with any type.
         * - A global provision of data on the specified key has already been requested.
         *
         * \sa registerProvide()
         *
         * A module can register a global provision of data with a certain key and type. A global
         * provision is slighly different from a normal provision, so that it can safely serve requests
         * between threads. In addition, it can serve already registered local requests on the same key,
         * which did not have a previous temporal provision. To do so, a global provision of data requires
         * an initial value to be setup on the key, so that at least a value is always available to be 
         * requested. In addition, to keep things simple and consistent, a single global provider is
         * allowed on a given key, with no other providers, be it local or global.
         * The request is accepted unless:
         *
         * - A local provider of data is already registered on the same key.
         * - A global provider of data is already registered on the same key.
         * - A request for data of a different type is already registered on the same key.
         *
         * \sa registerGlobalProvide()
         *
         * Finally, a module can register a global request for data. These request will arrive from 
         * modules of other threads, and thus must be served by global data provisions, as Blackboard
         * must guarantee that with any possible thread scheduling such a request will return some value.
         * Local or global data requests are treated the same, except for validation rules.
         * The request is accepted unless:
         *
         * - A local provider of data is already registered on the same key.
         * - A global provider of data is already registered on the same key, but with a different type.
         * - A request for data of a different type is already registered on the same key.
         *
         * \sa registerGlobalRequire()
         */
        class Blackboard : public Log::Loggable {
            public:
                /**
                 * @brief Basic constructor.
                 *
                 * Blackboard slighly modifies the name of the sink in order to avoid logging client
                 * conflicts. The name of the instance is however unchanged.
                 *
                 * @param name The name of the Blackboard.
                 */
                Blackboard(std::string name);

                /**
                 * @brief Basic destructor.
                 *
                 * Cleans the logging sink.
                 */
                ~Blackboard();

                // Cannot move Blackboard as it creates functions that point to it.
                Blackboard(Blackboard &&) = delete;
                const Blackboard & operator=(Blackboard &&) = delete;
                
                /**
                 * @brief This function registers a data request of the given type and key.
                 *
                 * This function returns a function that can be used to access the data.                  *
                 * The function returned returns a copy here, so you may want to store the result
                 * somewhere or it gets expensive. The reason is that we can't allow you to have
                 * a reference, otherwise you'd be bypassing the locking mechanism which prevents
                 * data-corruption.
                 *                                                                                      
                 * As an addendum, it is possible to create a different function that returns a pointer
                 * which is allocated if the key exists, or empty if it does not. Not sure if that
                 * can have any uses, we can possibly implement that at a later time.
                 *
                 * @tparam T The type of the data request.
                 * @param key The key that should hold the data.
                 * @param e A pointer to report eventual errors to the caller.
                 *
                 * @return A function to access data if successful, an empty function otherwise.
                 */
                template <class T>
                RequireFunction<T> registerRequire          (const std::string & key, RegistrationError * e = nullptr);

                /**
                 * @brief This function registers a local data provision with the given type and key.
                 *
                 * @tparam T The type of the data provided.
                 * @param key The key had should hold the data.
                 * @param e A pointer to report eventual errors to the caller.
                 *
                 * @return A function to set data if successful, an empty function otherwise.
                 */
                template <class T>
                ProvideFunction<T> registerProvide          (const std::string & key, RegistrationError * e = nullptr);

                /**
                 * @brief This function register a global data request with the given type and key.
                 *
                 * This function returns a function that can be used to access the data.
                 * The function returned returns a copy here, so you may want to store the result
                 * somewhere or it gets expensive. The reason is that we can't allow you to have
                 * a reference, otherwise you'd be bypassing the locking mechanism which prevents
                 * data-corruption.
                 *
                 * @tparam T The type of the data request.
                 * @param key The key that should hold the data.
                 * @param e A pointer to report eventual errors to the caller.
                 *
                 * @return A function to access data if successful, an empty function otherwise.
                 */
                template <class T>
                RequireFunction<T> registerGlobalRequire    (const std::string & key, RegistrationError * e = nullptr);

                /**
                 * @brief A function to register a global provision of data with the given type and key.
                 *
                 * This function not only registers a provide, but sets it, so that inter-thread
                 * requires don't break no matter the thread-order. Info that has to be read outside
                 * of the thread where it is provided should use this function.
                 *
                 * @tparam T The type of the data provided.
                 * @param key The key that should hold the data.
                 * @param data An initial value for the data.
                 * @param e A pointer to report eventual errors to the caller.
                 *
                 * @return A function to set data if successful, an empty function otherwise.
                 */
                template <class T>
                ProvideFunction<T> registerGlobalProvide    (const std::string & key, const T & data, RegistrationError * e = nullptr);

                /**
                 * @brief This function checks whether all data requests have been provided for.
                 *
                 * Since the initialization of global requires and global provides is not linear, this
                 * gives a way to discern whether the current configuration is OK under that side.
                 *
                 * @return 
                 */
                bool validateGlobals() const;

                /**
                 * @brief This function returns the name of the Blackboard.
                 *
                 * @return The name of the Blackboard.
                 */
                const std::string & getName() const;

            private:
                std::string name_;
                using Lock = boost::shared_mutex;
                using WriteLock = boost::unique_lock<Lock>;
                using ReadLock  = boost::shared_lock<Lock>;

                enum class TypeState {
                    Requested,          // A requested status can only be fixed by a global provider
                    Provided,           // If something is Provided and we get a GlobalRequest, error!
                    GlobalProvided      // Only a single global provider is allowed for a particular key.
                };

                template<class T>
                RequireFunction<T> makeRequireFunction(const std::string & key);
                template<class T>
                ProvideFunction<T> makeProvideFunction(const std::string & key);

                // This is the map that is actually used during a run of the framework.
                std::unordered_map<std::string, std::pair<boost::shared_mutex, boost::any>> board_;
                // First type index is used generally. We use two in case we request a type, and then 
                // we provide another. the first type then needs to wait for a global provide, or 
                // we cannot validate the arrangement.
                std::unordered_map<std::string, std::pair<TypeState,std::type_index>> typeCheck_;
        };

        template <class T>
        RequireFunction<T> Blackboard::registerRequire(const std::string & key, RegistrationError * e) {
            auto it = board_.find(key);
            std::type_index type(typeid(T));

            if ( it != std::end(board_) ) {
                auto & tuple = typeCheck_.at(key);
                // Check that whatever is in there is our type.
                if ( type != std::get<1>(tuple) ) {
                    if ( e ) *e = RegistrationError::WrongType;
                    return RequireFunction<T>();
                } // State remains as it was before. Requested -> Requested, Provided -> Provided, GlobalProvided -> GlobalProvided
            }
            else typeCheck_.emplace(key, std::make_pair(TypeState::Requested, type));

            // Creating accessor function
            return makeRequireFunction<T>(key);
        }

        template <class T>
        ProvideFunction<T> Blackboard::registerProvide(const std::string & key, RegistrationError * e) {
            log("Providing " + key);
            auto it = board_.find(key);
            std::type_index type(typeid(T));

            if ( it != std::end(board_) ) {
                log("    Already registered.");
                auto & pair = typeCheck_.at(key);
                // If it was requested, then the only way this is going to work is that the key gets GlobalProvided.
                // Since global providers are unique, we can't provide here.
                if ( std::get<0>(pair) == TypeState::Requested ) {
                    if ( e ) *e = RegistrationError::Requested;
                    return ProvideFunction<T>();
                }
                // If it was GloballyProvided, it's unique and we can't provide here.
                else if ( std::get<0>(pair) == TypeState::GlobalProvided ) {
                    if ( e ) *e = RegistrationError::GloballyProvided;
                    return ProvideFunction<T>();
                }
                // Otherwise just overwrite what was there.
                else std::get<1>(pair) = typeid(T);
            }
            else {
                log("    New registration.");
                typeCheck_.emplace(key,std::make_pair(TypeState::Provided, type));
            }

            // Setting up board key. We have to do this because record creation is not
            // protected by the mutexes, only the modifications are!
            log("Setting " + key);
            board_[key];

            // Creating accessor function.
            log("Building provider function.");
            return makeProvideFunction<T>(key);
        }

        template <class T>
        RequireFunction<T> Blackboard::registerGlobalRequire(const std::string & key, RegistrationError * e) {
            // Here we have the additional constraint that the key cannot be Provided.
            // Otherwise we cannot guarantee that whatever sheduling happens between threads will 
            // make sure that this require always succeeds.
            auto it = board_.find(key);
            std::type_index type(typeid(T));

            if ( it != std::end(board_) && std::get<0>(typeCheck_.at(key)) == TypeState::Provided ) {
                if ( e ) *e = RegistrationError::LocallyProvided;
                return RequireFunction<T>();
            }
            // Applies all local require constraints
            return registerRequire<T>(key);
        }

        template <class T>
        ProvideFunction<T> Blackboard::registerGlobalProvide(const std::string & key, const T & value, RegistrationError * e) {
            auto it = board_.find(key);
            std::type_index type(typeid(T));

            if ( it != std::end(board_) ) {
                auto & tuple = typeCheck_.at(key);
                auto & currentState = std::get<0>(tuple);

                if ( currentState == TypeState::Provided ) {
                    if ( e ) *e = RegistrationError::LocallyProvided;
                    return ProvideFunction<T>();
                }
                else if ( currentState == TypeState::GlobalProvided ) {
                    if ( e ) *e = RegistrationError::GloballyProvided;
                    return ProvideFunction<T>();
                }
                else if ( type != std::get<1>(tuple) ) {
                    if ( e ) *e = RegistrationError::WrongType;
                    return ProvideFunction<T>();
                }
                currentState = TypeState::GlobalProvided;
            }
            else typeCheck_.emplace(key, std::make_pair(TypeState::GlobalProvided, type));

            // Setting up board key. We have to do this because record creation is not
            // protected by the mutexes, only the modifications are!
            board_[key].second = value;

            return makeProvideFunction<T>(key);
        }

        template<class T>
        RequireFunction<T> Blackboard::makeRequireFunction(const std::string & key) {
            RequireFunction<T> requirer = [this, key](){
                auto & pair = board_.at(key);
                ReadLock lock(pair.first);

                return boost::any_cast<T>(pair.second);
            };
            return requirer;
        }

        template<class T>
        ProvideFunction<T> Blackboard::makeProvideFunction(const std::string & key) {
            ProvideFunction<T> provider = [this, key](const T& input){
                auto & pair = board_[key];
                WriteLock lock(pair.first);

                pair.second = input;
            };
            return provider;
        }
    }
}

#endif
