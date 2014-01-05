cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(dynamicGestures)

set(SSE_FLAGS "-msse3") 
add_definitions(${SSE_FLAGS})

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.52.0) 

include(dlib-18.5/dlib/cmake)
include_directories(${Boost_INCLUDE_DIRS} HMMlib-1.0.2)
add_executable(dyn preprocess/LMpre.cpp Model/GestureFinger.cpp Model/GestureFrame.cpp Model/GestureHand.cpp Model/Vertex.cpp StorageDriver/BinaryFileStorageDriver.cpp dynamicGestureRecognition.cpp)
target_link_libraries(dyn ${Boost_LIBRARIES} dlib)
