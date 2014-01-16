/*
 * hmmClass.h
 *
 *  Created on: 15-01-2014
 *      Author: smi
 */

#ifndef HMMCLASS_H_
#define HMMCLASS_H_

#include <HMMlib/hmm_table.hpp>
#include <HMMlib/hmm_vector.hpp>
#include <HMMlib/hmm.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "../KFoldCrossValidation.h"

using namespace hmmlib;
using namespace std;

class HMMClass {
private:
	HMMClass();
	HMM<double> *hmm;

public:
	HMMClass(int K, int N, int M);
	double predict(sequence test);

	void train(std::vector<sequence> trainDataset, int crossValK = 5, int iterationNumber = 1000, double learningRate = 0.1);

	void show();


	int K, N, M;
	boost::shared_ptr<HMMVector<double> > pi_ptr;
	boost::shared_ptr<HMMMatrix<double> > T_ptr;
	boost::shared_ptr<HMMMatrix<double> > E_ptr;
};



#endif /* HMMCLASS_H_ */
