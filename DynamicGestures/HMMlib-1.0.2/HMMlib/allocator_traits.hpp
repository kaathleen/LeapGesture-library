//==========================================================================
//  This file is part of HMMlib.
//
//  HMMlib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as
//  published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  HMMlib is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with HMMlib. If not, see
//  <http://www.gnu.org/licenses/>.
//
//  Copyright (C) 2010  Bioinformatics Research Centre, Aarhus University.
//  Author: Andreas Sand (asand@birc.au.dk)
//==========================================================================

#ifndef ALLOCATOR_TRAITS_HPP
#define ALLOCATOR_TRAITS_HPP

#include <pmmintrin.h>
#include <cstdlib>

namespace hmmlib {

  template<typename float_type, typename sse_float_type>
  class AllocatorTraits { 

  public :
    template<typename Table>
    static inline void allocate(int rows, int columns, Table &T) {
      T.no_chunks_per_row = columns;
      T.no_columns = T.no_allocated_columns = columns;
      T.no_rows = rows;
			
      T.table = static_cast<float_type *>( malloc(T.no_allocated_columns * T.no_rows * sizeof(float_type)));
    }
		
    template<typename Table>
    static inline void free(Table &T) {
      std::free(T.table);
    }
  };
	
  template<>
  class AllocatorTraits<double, __m128d> {
    typedef double float_type;
    typedef __m128d sse_float_type;

  public :		
    template<typename Table>
    static inline void allocate(int rows, int columns, Table &T) {
      int floats_per_chunk = sizeof(sse_float_type) / sizeof(float_type);
			
      if(columns % floats_per_chunk == 0)
	T.no_chunks_per_row = columns / floats_per_chunk; // integer division
      else
	T.no_chunks_per_row = (columns / floats_per_chunk) + 1;
      T.no_rows = rows;
      T.no_columns = columns;
      T.no_allocated_columns = T.no_chunks_per_row * floats_per_chunk;
			
      T.table = static_cast<float_type *>( _mm_malloc(T.no_allocated_columns * T.no_rows * sizeof(float_type), 16));
    }
		
    template<typename Table>
    static inline void free(Table &T) {
      _mm_free(T.table);
    }
  };
	
  template<>
  class AllocatorTraits<float, __m128> {
    typedef float float_type;
    typedef __m128 sse_float_type;
  public :	
    template<typename Table>
    static inline void allocate(int rows, int columns, Table &T) {
      int floats_per_chunk = sizeof(sse_float_type) / sizeof(float_type);
			
      if(columns % floats_per_chunk == 0)
	T.no_chunks_per_row = columns / floats_per_chunk; // integer division
      else
	T.no_chunks_per_row = (columns / floats_per_chunk) + 1;
      T.no_rows = rows;
      T.no_columns = columns;
      T.no_allocated_columns = T.no_chunks_per_row * floats_per_chunk;
			
      T.table = static_cast<float_type *>( _mm_malloc(T.no_allocated_columns * T.no_rows * sizeof(float_type), 16));
    }
		
    template<typename Table>
    static inline void free(Table &T) {
      _mm_free(T.table);
    }
  };
} // end of namespace

#endif
