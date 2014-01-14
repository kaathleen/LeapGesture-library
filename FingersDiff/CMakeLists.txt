cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(FingersDiff)

set(LibSVM_LIBRARIES svm)


add_executable (FingersDiff FingerDiff.cpp SVMclassificator.cpp Model/GestureFinger.cpp Model/GestureFrame.cpp Model/GestureHand.cpp Model/Vertex.cpp StorageDriver/BinaryFileStorageDriver.cpp Preprocessing/LMpre.cpp)
target_link_libraries(FingersDiff ${LibSVM_LIBRARIES})
