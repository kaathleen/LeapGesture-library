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

#include "HMMlib/hmm_table.hpp"
#include "HMMlib/hmm_vector.hpp"
#include "HMMlib/hmm.hpp"
using namespace hmmlib;

#include <boost/shared_ptr.hpp>
#include <boost/python/module.hpp>
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/overloads.hpp>
using namespace boost::python;
using namespace boost;

static void copy_matrix_to_lists(list &dst, const HMMMatrix<double> &src) {
  for(int r = 0; r < src.get_no_rows(); ++r) {
    list dst_r;
    for(int c = 0; c < src.get_no_columns(); ++c) {
      dst_r.append(src(r,c));
    }
    dst.append(dst_r);
  }
}

static void copy_lists_to_matrix(HMMMatrix<double> &dst, list &src) {
  for(int r = 0; r < len(src); ++r) {
    for(int c = 0; c < len(src[0]); ++c) {
      dst(r,c) = extract<double>(src[r][c]);
    }
  }
}

static void copy_vector_to_list(list &dst, const HMMVector<double> &src) {
  for(int i = 0; i < src.get_size(); ++i) {
    dst.append(src(i));
  }
}

static void copy_list_to_sequence(sequence &dst, const list &src) {
  for(int i = 0; i < len(src); ++i) {
    dst.push_back(extract<unsigned int>(src[i]));
  }
}

static void copy_sequence_to_list(list &dst, const sequence &src) {
  for(size_t i = 0; i < src.size(); ++i) {
    dst.append(src[i]);
  }
}

static void copy_list_to_vector(HMMVector<double> &dst, const list &src) {
  for(int i = 0; i < len(src); ++i) {
    dst(i) = extract<double>(src[i]);
  }
}

static double get_item_table(object self, object idxTuple) {
  HMMTable<double> &t = extract<HMMTable<double>&>(self);
  int i = extract<int>(idxTuple[0]);
  int j = extract<int>(idxTuple[1]);

  // negative indices
  if(i < 0)
    i = t.get_no_rows() - i;
  if(j < 0)
    j = t.get_no_columns() - j;

  return t(i,j);
}

static void set_item_table(object self, object idxTuple, object value) {
  HMMTable<double> &t = extract<HMMTable<double>&>(self);
  double s = extract<double>(value);
  int i = extract<int>(idxTuple[0]);
  int j = extract<int>(idxTuple[1]);

  // negative indices
  if(i < 0)
    i = t.get_no_rows() - i;
  if(j < 0)
    j = t.get_no_columns() - j;

  t(i,j) = s;
}

static double get_item_vector(object self, int i) {
  HMMVector<double> &v = extract<HMMVector<double>&>(self);

  // negative indices
  if(i < 0)
    i = v.get_size() - i;

  return v(i);
}

static void set_item_vector(object self, int i, object value) {
  HMMVector<double> &v = extract<HMMVector<double>&>(self);
  double s = extract<double>(value);

  // negative indices
  if(i < 0)
    i = v.get_size() - i;

  v(i) = s;
}

static unsigned int get_item_sequence(object self, int i) {
  sequence &s = extract<sequence&>(self);

  // negative indices
  if(i < 0)
    i = s.size() - i;

  return s[i];
}

static void set_item_sequence(object self, int i, object value) {
  sequence &s = extract<sequence&>(self);
  unsigned int v = extract<double>(value);

  // negative indices
  if(i < 0)
    i = s.size() - i;

  s[i] = v;
}

// static double likelihood_simple(object self, object obsseq_object) {
//   HMM<double> &hmm = extract<HMM<double>&>(self);
//   sequence obsseq = extract<sequence>(obsseq_object);
//   int length = obsseq.size();

//   HMMMatrix<double> f(length, hmm.get_no_states());
//   HMMVector<double> scales(length);

//   hmm.forward(obsseq, scales, f);
//   return hmm.likelihood(scales);
// }

static double likelihood_lists_from_obsseq(object self, object seq_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list seq_list = extract<list>(seq_list_object);
  int length = len(seq_list);

  sequence obsseq;
  copy_list_to_sequence(obsseq, seq_list);
 
  HMMMatrix<double> f(length, hmm.get_no_states());
  HMMVector<double> scales(length);

  hmm.forward(obsseq, scales, f);
  return hmm.likelihood(scales);
}

static double likelihood_lists_from_scales(object self, object scales_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list scales_list = extract<list>(scales_list_object);
  int length = len(scales_list);

  HMMVector<double> scales(length);
  for(int i = 0; i < length; ++i)
    scales(i) = extract<double>(scales_list[i]);

  return hmm.likelihood(scales);
}

// static boost::python::tuple viterbi_simple(object self, object obsseq_object) {
//   HMM<double> &hmm = extract<HMM<double>&>(self);
//   sequence obsseq = extract<sequence>(obsseq_object);

//   sequence hiddenseq(obsseq.size());
//   double loglik = hmm.viterbi(obsseq,hiddenseq);

//   return boost::python::make_tuple<sequence, double>(hiddenseq, loglik);
// }

static boost::python::tuple viterbi_lists(object self, object obsseq_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  int length = len(obsseq_list);

  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);
  
  sequence hiddenseq(length);
  double loglik = hmm.viterbi(obsseq,hiddenseq);

  list hiddenseq_list;
  copy_sequence_to_list(hiddenseq_list, hiddenseq);

  return boost::python::make_tuple<list, double>(hiddenseq_list, loglik);
}

// static boost::python::tuple forward_simple(object self, object obsseq_object) {
//   HMM<double> &hmm = extract<HMM<double>&>(self);
//   sequence obsseq = extract<sequence>(obsseq_object);
//   int length = obsseq.size();

//   HMMVector<double> scales(length);
//   HMMMatrix<double> f(length, hmm.get_no_states());

//   hmm.forward(obsseq, scales, f);

//   return boost::python::make_tuple<HMMVector<double>, HMMMatrix<double> >(scales, f);
// }


static boost::python::tuple forward_lists(object self, object obsseq_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();
  
  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);

  HMMVector<double> scales(length);
  HMMMatrix<double> f(length, no_states);

  hmm.forward(obsseq, scales, f);

  list f_list;
  copy_matrix_to_lists(f_list, f);
  list scales_list;
  copy_vector_to_list(scales_list, scales);

  return boost::python::make_tuple<list, list >(scales_list, f_list);
}


static list backward_lists(object self, object obsseq_list_object, object scales_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  list scales_list = extract<list>(scales_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();
  
  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);
  HMMVector<double> scales(length);
  copy_list_to_vector(scales, scales_list);

  HMMMatrix<double> B(length, no_states);
  
  hmm.backward(obsseq, scales, B);

  list B_list;
  copy_matrix_to_lists(B_list, B);

  return B_list;
}

static boost::python::tuple forward_and_backward(object self, object obsseq_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();

  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);

  HMMVector<double> scales(length);
  HMMMatrix<double> F(length, no_states);
  HMMMatrix<double> B(length, no_states);
  hmm.forward(obsseq, scales, F);
  hmm.backward(obsseq, scales, B);

  list F_list;
  copy_matrix_to_lists(F_list, F);
  list B_list;
  copy_matrix_to_lists(B_list, B);
  list scales_list;
  copy_vector_to_list(scales_list, scales);
  
  return boost::python::make_tuple<list,list,list>(F_list, B_list, scales_list);
}

static list posterior_decoding_lists_from_obsseq(object self, object obsseq_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();

  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);

  HMMVector<double> scales(length);
  HMMMatrix<double> F(length, no_states);
  HMMMatrix<double> B(length, no_states);
  HMMMatrix<double> post(length, no_states);
  hmm.forward(obsseq, scales, F);
  hmm.backward(obsseq, scales, B);
  hmm.posterior_decoding(obsseq, F, B, scales, post);

  list post_list;
  copy_matrix_to_lists(post_list, post);

  return post_list;
}

static list posterior_decoding_lists(object self, object obsseq_list_object, object F_list_object, object B_list_object, object scales_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  list F_list = extract<list>(F_list_object);
  list B_list = extract<list>(B_list_object);
  list scales_list = extract<list>(scales_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();

  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);

  HMMMatrix<double> F(length, no_states);
  copy_lists_to_matrix(F, F_list);

  HMMMatrix<double> B(length, no_states);
  copy_lists_to_matrix(B, B_list);

  HMMVector<double> scales(length);
  copy_list_to_vector(scales, scales_list);

  HMMMatrix<double> post(length, no_states);

  hmm.posterior_decoding(obsseq, F, B, scales, post);

  list post_list;
  copy_matrix_to_lists(post_list, post);

  return post_list;
}

static boost::python::tuple baum_welch_lists_from_obsseq(object self, object obsseq_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();
  int alphabet_size = hmm.get_alphabet_size();

  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);
  
  HMMVector<double> scales(length);
  HMMMatrix<double> F(length, no_states);
  HMMMatrix<double> B(length, no_states);

  hmm.forward(obsseq, scales, F);
  hmm.backward(obsseq, scales, B);

  HMMVector<double> pi_counts(no_states);
  HMMMatrix<double> T_counts(no_states, no_states);
  HMMMatrix<double> E_counts(alphabet_size, no_states);

  hmm.baum_welch(obsseq, F, B, scales, pi_counts, T_counts, E_counts);

  list pi_counts_list;
  copy_vector_to_list(pi_counts_list, pi_counts);

  list T_counts_list;
  copy_matrix_to_lists(T_counts_list, T_counts);

  list E_counts_list;
  copy_matrix_to_lists(E_counts_list, E_counts);
  
  return boost::python::make_tuple<list, list, list>(pi_counts_list, T_counts_list, E_counts_list);
}

static boost::python::tuple baum_welch_lists(object self, object obsseq_list_object, object F_list_object, object B_list_object, object scales_list_object) {
  HMM<double> &hmm = extract<HMM<double>&>(self);
  list obsseq_list = extract<list>(obsseq_list_object);
  list F_list = extract<list>(F_list_object);
  list B_list = extract<list>(B_list_object);
  list scales_list = extract<list>(scales_list_object);
  int length = len(obsseq_list);
  int no_states = hmm.get_no_states();
  int alphabet_size = hmm.get_alphabet_size();

  sequence obsseq;
  copy_list_to_sequence(obsseq, obsseq_list);

  HMMMatrix<double> F(length, no_states);
  copy_lists_to_matrix(F, F_list);

  HMMMatrix<double> B(length, no_states);
  copy_lists_to_matrix(B, B_list);

  HMMVector<double> scales(length);
  copy_list_to_vector(scales, scales_list);

  HMMVector<double> pi_counts(no_states);
  HMMMatrix<double> T_counts(no_states, no_states);
  HMMMatrix<double> E_counts(alphabet_size, no_states);

  hmm.baum_welch(obsseq, F, B, scales, pi_counts, T_counts, E_counts);

  list pi_counts_list;
  copy_vector_to_list(pi_counts_list, pi_counts);

  list T_counts_list;
  copy_matrix_to_lists(T_counts_list, T_counts);

  list E_counts_list;
  copy_matrix_to_lists(E_counts_list, E_counts);
  
  return boost::python::make_tuple<list, list, list>(pi_counts_list, T_counts_list, E_counts_list);
}

struct HMMTableCallback : HMMTable<double>, wrapper<HMMTable<double> > {
  
  HMMTableCallback(const int no_rows, const int no_columns, double val = 0.0) 
    :HMMTable<double>(no_rows, no_columns, val)
  { }

  ~HMMTableCallback() {
    this->get_override("~HMMTableCallback")();
  }

  HMMTableCallback &operator=(double val) {
    return this->get_override("operator=")(val);
  }
};

BOOST_PYTHON_MODULE(pyhmmlib) {
  class_<HMMTableCallback, shared_ptr<HMMTableCallback>,noncopyable>("HMMTable", no_init)
    .def(init<int, int>())
    .def(init<int, int, double>())
    .def("column_sum", &HMMTable<double>::column_sum)
    .def("row_sum", &HMMTable<double>::row_sum)
    .def("reset", &HMMTable<double>::reset)
    .def("get_no_rows", &HMMTable<double>::get_no_rows)
    .def("get_no_columns", &HMMTable<double>::get_no_columns)
    .def(self += double())
    .def(self -= double())
    .def(self *= double())
    .def(self /= double())
    .def("__setitem__", set_item_table)
    .def("__getitem__", get_item_table)
    ;

  class_<HMMMatrix<double>, shared_ptr<HMMMatrix<double> >, bases<HMMTable<double> >, noncopyable>("HMMMatrix", no_init)
    .def(init<int, int>())
    .def(init<int, int, double>())
    .def("transpose", &HMMMatrix<double>::transpose)
    ;

  class_<HMMVector<double>, shared_ptr<HMMVector<double> >, bases<HMMTable<double> >, noncopyable>("HMMVector", no_init)
    .def(init<int>())
    .def(init<int, double>())
    .def("__getitem__", get_item_vector)
    .def("__setitem__", set_item_vector)
    .def("sum", &HMMVector<double>::sum)
    .def("get_size", &HMMVector<double>::get_size)
    ;

  class_<sequence>("Sequence", no_init)
    .def(init<int>())
    .def("__setitem__", set_item_sequence)
    .def("__getitem__", get_item_sequence)
    .def("__len__", &sequence::size)
    .def("len", &sequence::size)
    ;

  class_<HMM<double>, noncopyable>("HMM", no_init)
    .def(init<shared_ptr<HMMVector<double> >, 
	 shared_ptr<HMMMatrix<double> >, 
	 shared_ptr< HMMMatrix<double> > >())
    .def("get_alphabet_size", &HMM<double>::get_alphabet_size)
    .def("get_no_states", &HMM<double>::get_no_states)
    .def("forward", &HMM<double>::forward)
    .def("forward_lists", &forward_lists)
    .def("backward", &HMM<double>::backward)
    .def("backward_lists", &backward_lists)
    .def("forward_and_backward", &forward_and_backward)
    .def("viterbi", &HMM<double>::viterbi)
    .def("viterbi_lists", &viterbi_lists)
    .def("likelihood", &HMM<double>::likelihood)
    .def("likelihood_lists_from_obsseq", &likelihood_lists_from_obsseq)
    .def("likelihood_lists_from_scales", &likelihood_lists_from_scales)
    .def("baum_welch", &HMM<double>::baum_welch)
    .def("baum_welch_lists", &baum_welch_lists)
    .def("baum_welch_lists_from_obsseq", &baum_welch_lists_from_obsseq)
    .def("posterior_decoding", &HMM<double>::posterior_decoding)
    .def("posterior_decoding_lists", &posterior_decoding_lists)
    .def("posterior_decoding_lists_from_obsseq", &posterior_decoding_lists_from_obsseq)
    ;

}
