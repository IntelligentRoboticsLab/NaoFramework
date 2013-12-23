#ifndef NAO_FRAMEWORK_COMM_BLACKBOARD_HEADER_FILE
#define NAO_FRAMEWORK_COMM_BLACKBOARD_HEADER_FILE

#include <NaoFramework/Comm/Types.hpp>
#include <NaoFramework/Log/Frontend.hpp>

#include <unordered_map>
#include <functional>
#include <typeindex>

#include <boost/any.hpp>
#include <boost/thread.hpp>

namespace NaoFramework {
    namespace Comm {
        // A Blackboard object allows in-out communication for a single thread, and out-only
        // communication from a thread to many threads. The object allows registration of data
        // requests and data provisions of any sizes. 
        //
        // Blackboard is able to perform run-time type checking. It does so in two different ways.
        // 
        // On one side there is the local-communication type check. This requires each request and
        class Blackboard {
            public:
                Blackboard(std::string name);
                ~Blackboard();
                // The function returned returns a copy here, so you may want to store the result
                // somewhere or it gets expensive. The reason is that we can't allow you to have
                // a reference, otherwise you'd be bypassing the locking mechanism which prevents
                // data-corruption.
                //
                // As an addendum, it is possible to create a different function that returns a pointer
                // which is allocated if the key exists, or empty if it does not. Not sure if that
                // can have any uses, we can possibly implement that at a later time.
                template <class T>
                RequireFunction<T> registerRequire          (const std::string &, RegistrationError * e = nullptr);

                // The function returned here takes a T value and overwrites the appropriate key
                // with it.
                template <class T>
                ProvideFunction<T> registerProvide          (const std::string &, RegistrationError * e = nullptr);

                // This function is called by other threads in order to check for 
                template <class T>
                RequireFunction<T> registerGlobalRequire    (const std::string &, RegistrationError * e = nullptr);

                // This function not only registers a provide, but sets it, so that inter-thread
                // requires don't break no matter the thread-order. Info that has to be read outside
                // of the thread where it is provided should use this function.
                template <class T>
                ProvideFunction<T> registerGlobalProvide    (const std::string &, const T &, RegistrationError * e = nullptr);

                // Since the initialization of global requires and global provides is not linear, this
                // gives a way to discern whether the current configuration is OK under that side.
                bool validateGlobals() const;

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
            Log::log(name_, "Providing " + key);
            auto it = board_.find(key);
            std::type_index type(typeid(T));

            if ( it != std::end(board_) ) {
                Log::log(name_, "    Already registered.");
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
                Log::log(name_, "    New registration.");
                typeCheck_.emplace(key,std::make_pair(TypeState::Provided, type));
            }

            // Setting up board key. We have to do this because record creation is not
            // protected by the mutexes, only the modifications are!
            Log::log(name_, "Setting " + key);
            board_[key];

            // Creating accessor function.
            Log::log(name_, "Building provider function.");
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

            return registerRequire<T>(key);
        }

        template <class T>
        ProvideFunction<T> Blackboard::registerGlobalProvide(const std::string & key, const T & value, RegistrationError * e) {
            auto it = board_.find(key);
            std::type_index type(typeid(T));

            if ( it != std::end(board_) ) {
                auto & currentState = std::get<0>(typeCheck_.at(key));
                if ( currentState == TypeState::Provided ) {
                    if ( e ) *e = RegistrationError::LocallyProvided;
                    return ProvideFunction<T>();
                }
                else if ( currentState == TypeState::GlobalProvided ) {
                    if ( e ) *e = RegistrationError::GloballyProvided;
                    return ProvideFunction<T>();
                }
                currentState = TypeState::GlobalProvided;
            }
            else typeCheck_.emplace(key, std::make_pair(TypeState::GlobalProvided, type));

            // Setting up board key. We have to do this because record creation is not
            // protected by the mutexes, only the modifications are!
            board_[key] = std::make_pair( {}, value );

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
