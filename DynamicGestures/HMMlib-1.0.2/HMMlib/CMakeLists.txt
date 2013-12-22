##==========================================================================
##  This file is part of HMMlib.
##
##  HMMlib is free software: you can redistribute it and/or modify
##  it under the terms of the GNU Lesser General Public License as
##  published by the Free Software Foundation, either version 3 of
##  the License, or (at your option) any later version.
##
##  HMMlib is distributed in the hope that it will be useful, but
##  WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
##  Lesser General Public License for more details.
##
##  You should have received a copy of the GNU Lesser General Public
##  License along with HMMlib. If not, see
##  <http://www.gnu.org/licenses/>.
##
##  Copyright (C) 2010  Bioinformatics Research Centre, Aarhus University.
##  Author: Andreas Sand (asand@birc.au.dk)
##==========================================================================

file(GLOB hmmlib_sources *.hpp)
include_directories(${HMMlib_SOURCE_DIR})

# Install headers
INSTALL(FILES ${hmmlib_sources} DESTINATION include/HMMlib)