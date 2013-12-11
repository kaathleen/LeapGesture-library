cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(FingersDiff)

set(LibSVM_LIBRARIES svm)


add_executable (SVMstatic LMReader.cpp SVMclassificator.cpp static.cpp)
target_link_libraries(SVMstatic ${LibSVM_LIBRARIES})
