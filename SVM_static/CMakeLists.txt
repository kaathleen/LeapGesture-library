cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(SVMstatic)

set(LibSVM_LIBRARIES svm)


add_executable (SVMstatic preprocess/LMpre.cpp Model/GestureFinger.cpp Model/GestureFrame.cpp Model/GestureHand.cpp Model/Vertex.cpp StorageDriver/BinaryFileStorageDriver.cpp LMReader.cpp SVMclassificator.cpp static.cpp)
target_link_libraries(SVMstatic ${LibSVM_LIBRARIES})
