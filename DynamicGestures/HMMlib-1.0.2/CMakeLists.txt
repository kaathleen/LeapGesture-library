##    ========================================================================
##    This file is part of HMMlib.
##
##    HMMlib is free software: you can redistribute it and/or modify
##    it under the terms of the GNU Lesser General Public License as
##    published by the Free Software Foundation, either version 3 of
##    the License, or (at your option) any later version.
##
##    HMMlib is distributed in the hope that it will be useful, but
##    WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
##    Lesser General Public License for more details.
##
##    You should have received a copy of the GNU Lesser General Public
##    License along with HMMlib. If not, see
##    <http://www.gnu.org/licenses/>.
##
##    Copyright (C) 2010  Bioinformatics Research Centre, Aarhus University.
##    Author: Andreas Sand (asand@birc.au.dk)
##    ========================================================================

cmake_minimum_required(VERSION 2.6)

IF(UNIX AND NOT WIN32)
  message("
   
    ====================================================================================
    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program. If not, see
    <http://www.gnu.org/licenses/>.

    Copyright (C) 2010  Bioinformatics Research Centre, Aarhus University.
    Author: Andreas Sand (asand@birc.au.dk)
    ====================================================================================
    ")

##  set(CMAKE_CXX_COMPILER "g++")
##  set(CMAKE_C_COMPILER "gcc")

  project(HMMlib CXX C)

  ## Version
  set(HMMlib_VERSION_CURRENT "1")
  set(HMMlib_VERSION_REVISION "0")
  set(HMMlib_VERSION_AGE "2")

  # Set the CMAKE_PREFIX_PATH for the find_library function when using non
  # standard install location
  IF(CMAKE_INSTALL_PREFIX)
    set(CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}" ${CMAKE_PREFIX_PATH})
  ENDIF(CMAKE_INSTALL_PREFIX)

  # openmp
  if(WITH_OMP)
    find_package(OpenMP)
  endif(WITH_OMP)

  # Doxygen
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} Doxyfile)
  endif(DOXYGEN_FOUND)

  # Boost
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  find_package(Boost 1.42 QUIET COMPONENTS unit_test_framework python)
  if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_libraries(${Boost_LIBRARIES})
    message("-- Boost version: ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
    message("--  Boost include directory: ${Boost_INCLUDE_DIRS}") 
    message("--  Boost lib directory: ${Boost_LIBRARY_DIRS}") 
    message("--  Boost libraries: ${Boost_LIBRARIES}") 

    if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
      message("--  Boost.Test: yes")
    else(Boost_UNIT_TEST_FRAMEWORK_FOUND)
      message("--  Boost.Test: no")
      message("--    Test code will not be compiled.")
    endif(Boost_UNIT_TEST_FRAMEWORK_FOUND)

    if(Boost_PYTHON_FOUND)
      message("--  Boost.Python: yes")
    else(Boost_PYTHON_FOUND)
      message("--  Boost.Python: no")
      message("--    Python bindings will not be generated.")
    endif(Boost_PYTHON_FOUND)

  else (Boost_FOUND)
    message(FATAL_ERROR "The Boost C++ libraries was not found. Get Boost from http://www.boost.org/ or set the environment variable BOOST_ROOT to point to the root of boost directory.")
  endif (Boost_FOUND)

  # Python
  find_package(PythonLibs QUIET)
  if(PYTHONLIBS_FOUND)
    message("-- Python: yes")
    message("--  Python include directory: ${PYTHON_INCLUDE_PATH}")
    message("--  Python libraries: ${PYTHON_LIBRARIES}")
    include_directories(${PYTHON_INCLUDE_PATH})
    link_libraries(${PYTHON_LIBRARIES})
  else(PYTHONLIBS_FOUND)
    message("-- Python: no")
    message("--  Python bindings will not be generated.")
  endif(PYTHONLIBS_FOUND)

  set(CMAKE_CXX_FLAGS "-Wall -Wconversion -O3 ${OpenMP_CXX_FLAGS} -msse4")
  #  message("CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

  enable_testing()

  # Add subdirectories
  add_subdirectory(HMMlib)
  
  if(Boost_PYTHON_FOUND AND PYTHONLIBS_FOUND)
    add_subdirectory(python)
  endif(Boost_PYTHON_FOUND AND PYTHONLIBS_FOUND)

  if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    add_subdirectory(tests)
  endif(Boost_UNIT_TEST_FRAMEWORK_FOUND)
  
  # Effective version number computation
  set(HMMlib_VERSION_MAJOR "${HMMlib_VERSION_CURRENT}")
  set(HMMlib_VERSION_MINOR "${HMMlib_VERSION_REVISION}")
  set(HMMlib_VERSION_PATCH "${HMMlib_VERSION_AGE}")
  set(HMMlib_VERSION "${HMMlib_VERSION_MAJOR}.${HMMlib_VERSION_MINOR}.${HMMlib_VERSION_PATCH}")
  
  # Packager
  set(CPACK_PACKAGE_NAME "hmmlib")
  set(CPACK_PACKAGE_VENDOR "BiRC - Bioinformatics Research Center, Aarhus University, Denmark")
  set(CPACK_PACKAGE_VERSION "${HMMlib_VERSION_CURRENT}.${HMMlib_VERSION_REVISION}.${HMMlib_VERSION_AGE}")
  set(CPACK_PACKAGE_VERSION_MAJOR "${HMMlib_VERSION_CURRENT}")
  set(CPACK_PACKAGE_VERSION_MINOR "${HMMlib_VERSION_REVISION}")
  set(CPACK_PACKAGE_VERSION_PATCH "${HMMlib_VERSION_AGE}")

  IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_64)
    set(CPACK_TOPLEVEL_TAG x86_64)
  ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_32)
    set(CPACK_TOPLEVEL_TAG x86_32)
  ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")

  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HMMlib: a library for general hidden Markov Models, exploiting modern CPUs")
  set(CPACK_RESOURCE_FILE_LICENSE "COPYING")
  set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
  set(CPACK_SOURCE_IGNORE_FILES
    ".*CMakeFiles"
    ".*Makefile"
    ".*Makefile_manual"
    ".*_CPack_Packages"
    ".*CMakeCache.txt"
    ".*\\\\.cmake"
    ".*\\\\.svn/"
    "doc"
    ".*.gz"
    ".*.zip"
    "Testing"
    "tests/hmm_test$"
    "tests/hmm_table_test$"
    "tests/hmm_vector_test$"
    "tests/hmm_matrix_test$"
    "install_manifest.txt"
    ".svnignore.txt"
    "tests/.svnignore.txt"
    "HMMlib/.svnignore.txt"
    "python/.svnignore.txt"
    "python/pyhmmlib.so"
    "python/python_test.py"
    ".*~"
    ${CPACK_SOURCE_IGNORE_FILES}
    )
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  include(CPack)
  
  #This adds the 'dist' target
  add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
ELSE(UNIX AND NOT WIN32)
  message("
    ================================================
    HMMlib 1.0.2 does not support Microsoft Windows.
    ================================================
    ")
ENDIF(UNIX AND NOT WIN32)
