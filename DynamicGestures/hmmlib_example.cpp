#include <HMMlib/hmm_table.hpp>
#include <HMMlib/hmm_vector.hpp>
#include <HMMlib/hmm.hpp>

using namespace hmmlib;

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

int main(int argc, char **args) {
  int K = 2; // number of states in one gesture
  int M = 2; // number of possible observations
  int n = 12; // length of observed sequence
  
  boost::shared_ptr<HMMVector<double> > pi_ptr(new HMMVector<double>(K));
  boost::shared_ptr<HMMMatrix<double> > T_ptr(new HMMMatrix<double>(K,K));
  boost::shared_ptr<HMMMatrix<double> > E_ptr(new HMMMatrix<double>(M, K));
		
  boost::shared_ptr<HMMVector<double> > pi2_ptr(new HMMVector<double>(K));
  boost::shared_ptr<HMMMatrix<double> > T2_ptr(new HMMMatrix<double>(K,K));
  boost::shared_ptr<HMMMatrix<double> > E2_ptr(new HMMMatrix<double>(M,K));

  HMMVector<double> &pi = *pi_ptr;
  // initial probabilities
  pi(0)  = 0.5; pi(1) = 0.5;

  HMMMatrix<double> &T = *T_ptr;
  // transitions from state 0
  T(0,0) = 0.5; T(0,1) = 0.5;
  // transitions from state 1
  T(1,0) = 0.5; T(1,1) = 0.5;
	
  HMMMatrix<double> &E = *E_ptr;
  // emissions from state 0
  E(0,0) = 0.3; E(1,0) = 0.7;
  // emissions from state 1
  E(0,1) = 0.7; E(1,1) = 0.3;

  HMMVector<double> &pi2 = *pi2_ptr;
  HMMMatrix<double> &T2 = *T2_ptr;
  HMMMatrix<double> &E2 = *E2_ptr;
		
  //std::cout << "obs : [0, 1, 0, 1]" << std::endl;
  std::vector<sequence> learning_set;


  srand(time(0));
  for(int k=0;k<10;k++)
  {

	  sequence obs(n);
	  obs[0] = 0; 
	  obs[1] = 1;
	  obs[2] = 0; 
	  obs[3] = 1;
	  obs[4] = 0; 
	  obs[5] = 1;
	  obs[6] = 0;  
	  obs[7] = 1;
	  obs[8] = 0; 
	  obs[9] = 1;
	  obs[10] = 0; 
	  obs[11] = 1;

	  // Random noise
	  // Changing some value -> right now turned off
	  if ( rand()%10 > 10 )
	  {
		obs[round(rand()%12)] = 1 - obs[round(rand()%12)];
	  } 

	  std::cout << "obs length: " << obs.size() << std::endl;
   	  learning_set.push_back(obs);
  }

  sequence hiddenseq(n);
   HMM<double> *hmm;

  HMMMatrix<double> F(n, K);
  HMMVector<double> scales(n);
  HMMMatrix<double> B(n, K);
  HMMMatrix<double> pd(n, K);

  hmm = new HMM<double>(pi_ptr, T_ptr, E_ptr);

  int iteration_number = 10;
  double total_best = 0.0;
  double learn_test_percent = 0.5;
  for ( int i=0; i<iteration_number;i++)
  {
	for (int p = 0; p< learn_test_percent * learning_set.size(); p++)
	{
		double loglik = hmm->viterbi(learning_set[p], hiddenseq);
	  	std::cout << "-- log likelihood of hiddenseq: " << loglik << "\tLikelihood : " << exp(loglik) << std::endl;
		//std::cout << "Running forward" << std::endl;
	
		hmm->forward(learning_set[p], scales, F);

		//std::cout << "Running likelihood" << std::endl;
		loglik = hmm->likelihood(scales);

		//std::cout << "Running backward" << std::endl;
		hmm->backward(learning_set[p], scales, B);

		//std::cout << "Running posterior decoding" << std::endl;
		hmm->posterior_decoding(learning_set[p], F, B, scales, pd);

		//std::cout << "Running Baum-Welch" << std::endl;
		hmm->baum_welch(learning_set[p], F, B, scales, *pi2_ptr, *T2_ptr, *E2_ptr);

		delete hmm;
		for (int j=0;j<K;j++)
		{
			pi(j) = pi2(j);

			for (int k=0;k<M;k++)
			{
				E(k,j) = E2(k,j);
			}
			for (int k=0;k<K;k++)
			{
				T(k,j) = T2(k,j);
			}
		}
		hmm = new HMM<double>(pi_ptr, T_ptr, E_ptr);
	}

	double total_rec = 0;
	for (int p = learn_test_percent * learning_set.size(); p< learning_set.size(); p++)
	{
		double loglik = hmm->viterbi(learning_set[p], hiddenseq);
	  	total_rec += exp(loglik);
	}

	if ( total_rec <= total_best )
		break;
	
	total_best = total_rec;	
  }

  std::cout<<" ------ " << std::endl;
  std::cout<<" Proposed model ! " << std::endl;
  std::cout<<" -> Initial probabilites " << std::endl;	
  std::cout << (pi)(0) << " " << (pi)(1) << std::endl;

  std::cout<<" -> Transition matrix " << std::endl;	
  std::cout << (T)(0,0) << " " << (T)(0,1) << std::endl;
  std::cout << (T)(1,0) << " " << (T)(1,1) << std::endl;

  std::cout<<" -> Emission matrix " << std::endl;	
  std::cout << (E)(0,0) << " " << (E)(0,1) << std::endl;
  std::cout << (E)(1,0) << " " << (E)(1,1) << std::endl;

}
