cmake_minimum_required (VERSION 2.6)

add_library(NaoFramework DynamicModule.cpp Console.cpp)
target_link_libraries(NaoFramework dl readline)

add_executable(framework_test main.cpp)

target_link_libraries(framework_test NaoFramework) 
