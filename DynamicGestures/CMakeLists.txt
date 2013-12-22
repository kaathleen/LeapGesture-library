cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(dynamicGestures)

set(SSE_FLAGS "-msse3") 
add_definitions(${SSE_FLAGS})

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.52.0) 


include_directories(${Boost_INCLUDE_DIRS})
add_executable(dyn hmmlib_example.cpp)
target_link_libraries(dyn ${Boost_LIBRARIES})
