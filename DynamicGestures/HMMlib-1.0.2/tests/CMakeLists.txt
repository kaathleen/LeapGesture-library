##=========================================================================
## This file is part of HMMlib.
##
## HMMlib is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as
## published by the Free Software Foundation, either version 3 of
## the License, or (at your option) any later version.
##
## HMMlib is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with HMMlib. If not, see
## <http://www.gnu.org/licenses/>.
##
## Copyright (C) 2010  Bioinformatics Research Centre, Aarhus University.
## Author: Andreas Sand (asand@birc.au.dk)
##=========================================================================

include_directories(${HMMlib_SOURCE_DIR})
link_directories (${HMMlib_BINARY_DIR})

add_executable(hmm_matrix_test hmm_matrix_test.cpp)
add_test(hmm_matrix_test hmm_matrix_test)

add_executable(hmm_vector_test hmm_vector_test.cpp)
add_test(hmm_vector_test hmm_vector_test)

add_executable(hmm_test hmm_test.cpp)
add_test(hmm_test hmm_test)

if(OpenMP_CXX_FLAGS)
  set_property(
    SOURCE hmm_test.cpp
    PROPERTY COMPILE_DEFINITIONS WITH_OMP
    )
ENDIF(OpenMP_CXX_FLAGS)
