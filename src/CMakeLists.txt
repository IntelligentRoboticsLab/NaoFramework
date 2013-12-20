cmake_minimum_required (VERSION 2.6)

# External libraries we need
find_package(Boost COMPONENTS system thread log REQUIRED)
find_package(Readline REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

# Required by Boost::Log to link with shared libraries
add_definitions(-DBOOST_ALL_DYN_LINK)
add_library(NaoFramework DynamicModule.cpp Console.cpp ModuleInterface.cpp LogFrontend.cpp)
# Uppercase conventions here are different unfortunately..
target_link_libraries(NaoFramework dl ${READLINE_LIBRARY} ${Boost_LOG_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} pthread)

add_executable(framework_test main.cpp)

target_link_libraries(framework_test NaoFramework) 
