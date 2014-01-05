#include <HMMlib/hmm_table.hpp>
#include <HMMlib/hmm_vector.hpp>
#include <HMMlib/hmm.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "KFoldCrossValidation.h"

#include <dlib/clustering.h>
#include <dlib/rand.h>

#include "Model/GestureFinger.h"
#include "Model/GestureFrame.h"
#include "Model/GestureHand.h"
#include "Model/Vertex.h"

#include "StorageDriver/BinaryFileStorageDriver.h"
#include "StorageDriver/GestureStorageDriver.h"

#include "PathUtil.h"



using namespace hmmlib;
using namespace std;

#define MAX_FINGER_COUNT 5

// Should return a list of centroids (multiple per each class)
void kmeans(std::vector<std::vector<double> > data, int classCounter = 13,
		int maxCentroidsPerOneClass = 8, double kernelParam = 0.1, double error = 0.01) {
	typedef dlib::matrix<double> sample_type;
	typedef dlib::radial_basis_kernel<sample_type> kernel_type;

	// The first argument to the constructor is the kernel we wish to
	// use.
	// The second is a parameter that determines the numerical accuracy with which
	// the object will perform part of the learning algorithm.  Generally, smaller values
	// give better results but cause the algorithm to attempt to use more dictionary vectors
	// (and thus run slower and use more memory).
	// The third argument, however, is the
	// maximum number of dictionary vectors a kcentroid is allowed to use.  So you can use
	// it to control the runtime complexity.
	dlib::kcentroid<kernel_type> kc(kernel_type(kernelParam), error, maxCentroidsPerOneClass);

	// Now we make an instance of the kkmeans object and tell it to use kcentroid objects
	// that are configured with the parameters from the kc object we defined above.
	dlib::kkmeans<kernel_type> kmeans(kc);

	std::vector<sample_type> samples;
	std::vector<sample_type> initial_centers;

	// Copying to compatible type
	for (int i = 0; i < data.size(); i++) {
		sample_type m;
		m.set_size(data[i].size(),1);

		for (int j = 0; j < data[i].size(); j++) {
			m(j) = data[i][j];
		}
		samples.push_back(m);
	}

	// tell the kkmeans object we made that we want to run k-means with k set to 13.
	// (i.e. we want 13 clusters)
	kmeans.set_number_of_centers(classCounter);

	// You need to pick some initial centers for the k-means algorithm.  So here
	// we will use the dlib::pick_initial_centers() function which tries to find
	// n points that are far apart (basically).
	pick_initial_centers(classCounter, initial_centers, samples, kmeans.get_kernel());

	// now run the k-means algorithm on our set of samples.
	kmeans.train(samples, initial_centers);

	// To test one example:
	// label = test(samples[i])

	// Now print out how many dictionary vectors each center used.  Note that
	// the maximum number of 8 was reached.  If you went back to the kcentroid
	// constructor and changed the 8 to some bigger number you would see that these
	// numbers would go up.  However, 8 is all we need to correctly cluster this dataset.
	for (int i = 0; i < classCounter; i++) {
		cout << "num dictionary vectors for center " << i<<" : "
				<< kmeans.get_kcentroid(i).dictionary_size() << endl;
	}
}

// Should return a list of centroids (1 per class)
void kmeansSimple(std::vector< std::vector<double> > data, int classCounter = 13) {

	typedef dlib::matrix<double> sample_type;

	std::vector<sample_type> samples;
	std::vector<sample_type> initial_centers;

	// Copying to compatible type
	for (int i = 0; i < data.size(); i++) {
		sample_type m;
		m.set_size(data[i].size(),1);

		for (int j = 0; j < data[i].size(); j++) {
			m(j) = data[i][j];
		}
		samples.push_back(m);
	}

	typedef dlib::radial_basis_kernel<sample_type> kernel_type;
	pick_initial_centers(classCounter, initial_centers, samples, kernel_type(0.1));
	dlib::find_clusters_using_kmeans(samples, initial_centers);

	for (int i = 0; i < initial_centers.size(); i++) {
		cout << "Centroid " << i<<" :: " <<trans(initial_centers[i]) << "--------" << endl;
	}
}

// Maybe define it differently ?
double similarity(std::vector<double> a, std::vector<double> b)
{
	if (a.size() != b.size())
		return 1.0/0.00000001;

	double dist = 0.0;

	for (int k = 0; k < a.size(); k++) {
		dist += pow(a[k] - b[k], 2);
	}
	if (dist < 0.0001) {
		dist = 0.0001;
	}
	return 10.0 / dist;
}

// Can return labeled data, but cannot label new examples
void newman_cluster(std::vector<std::vector<double> > data, double eps = 0.0001,
		unsigned long max_iteration = 2000) {

	// calculating the similarity measure between examples
	std::vector<dlib::ordered_sample_pair> ord_samples;
	for (int i = 0; i < data.size(); i++) {
		for (int j = i+1; j < data.size(); j++) {

			dlib::ordered_sample_pair t(i, j, similarity(data[i], data[j]));
			ord_samples.push_back(t);
		}
	}
	cout<<"Running newman clustering"<<endl;
	std::vector<unsigned long> labels;
	dlib::newman_cluster(ord_samples, labels, eps, max_iteration);


	sort(labels.begin(), labels.end());
	unique(labels.begin(), labels.end());
	cout<<"Newman found :" <<labels.size() <<" classes" << endl;
}

// Can return labeled, but cannot label new examples
void chinese_whispers(std::vector< std::vector<double> > data, unsigned long numberOfIteration = 100) {
	// calculating the similarity measure between examples
	std::vector<dlib::ordered_sample_pair> ord_samples;
	for (int i = 0; i < data.size(); i++) {
		for (int j = i + 1; j < data.size(); j++) {

			dlib::ordered_sample_pair t(i, j, similarity(data[i], data[j]));
			ord_samples.push_back(t);
		}
	}
	std::vector<unsigned long> labels;
	dlib::chinese_whispers(ord_samples, labels, numberOfIteration);

	sort(labels.begin(), labels.end());
	unique(labels.begin(), labels.end());
	cout<<"Chinese whispers found :" <<labels.size() <<" classes" << endl;
}

int getIndexOfClassName(vector<string> &classNames, string name)
{
	for (int i=0; i<classNames.size(); i++)
	{
		if (classNames[i].compare(name) == 0)
			return i;
	}

	return -1;
}


void addAttribute(float attributeValue, int& attributeCounter,
		vector<double> &attributes) {
	attributes.push_back(attributeValue);
	attributeCounter++;
}

void fingerCountAttribute(int& fingerCount, int& attributeCounter,
		vector<double>& result) {
	addAttribute(fingerCount, attributeCounter, result);
}

void anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {


		vector<float> angles(6, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex fingerDirection = tempHand->getFinger(i)->getDirection();

			for (int j= i+1 ;j < fingerCount; j++) {

				Vertex fingerDirection2 = tempHand->getFinger(j)->getDirection();
				float angle = fingerDirection.dotProduct(fingerDirection2);
				angles.push_back(angle);// Can be acos if somebody want it
			}
		}

		sort(angles.begin(), angles.end(), std::greater<float>());

		for (int i = 0; i < 6; i++) {
			addAttribute(angles[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 6; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}


void distancesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {


		vector<float> distances(6, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex tipPosition = tempHand->getFinger(i)->getStabilizedTipPosition();

			for (int j= i+1 ;j < fingerCount; j++) {

				Vertex tipPosition2 = tempHand->getFinger(j)->getStabilizedTipPosition();
				float dist = (tipPosition - tipPosition2).getMagnitude();
				distances.push_back(dist);
			}
		}

		sort(distances.begin(), distances.end(), std::greater<float>());

		for (int i = 0; i < 6; i++) {
			addAttribute(distances[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 6; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}

// Computes the feature set
vector<double> computeFeatureSet(GestureFrame *gestureFrame) {
	vector<double> result;

	// Get the numbe of fingers
	GestureHand *tempHand = gestureFrame->getHand(0);
	int fingerCount =
			(tempHand != NULL) ?
					min(tempHand->getFingerCount(), MAX_FINGER_COUNT) : 0;
	int attributeCounter = 1;

	// Adding the finger count to the feature set
	fingerCountAttribute(fingerCount, attributeCounter, result);

	// Adding the 6 highest angles to the feature set
	anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result);

	// Adding the 6 greatest distances to the feature set
	distancesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result);

	return result;
}


int main(int argc, char **argv) {

	GestureStorageDriver* gestureStorageDriver = new BinaryFileStorageDriver();

	// Saving all possible gestures as feature sets
	vector< vector<double> > dataset;

	for (int i=1;i<argc;i++)
	{
		string fileName = getFileNameFromPath(argv[1]);

		gestureStorageDriver->openConnection(argv[1], false);
		cout << "Open connection for " << argv[1] << endl;
		GestureFrame currGestureFrame;

		while (gestureStorageDriver->loadGestureFrame(currGestureFrame)) {
			vector<double> row;
			row = computeFeatureSet(&currGestureFrame);

			dataset.push_back(row);
			currGestureFrame.clear();
		}
		gestureStorageDriver->closeConnection();
		cout << "Close connection" << endl;
	}
	delete gestureStorageDriver;


	// dataset contains all positions remember as featureSet
	cout<<"Size of dataset: " << dataset[0].size() << " x " << dataset.size()<<endl;

	/*
	// save dataset for counting number of clusters
	std::fstream datasetFile;
	datasetFile.open("DATASET_FILE", std::fstream::out | std::fstream::trunc);
	for (int i=0; i<dataset.size(); i++)
	{
		int lastElement = dataset[i].size()-1;
		for (int j=0; j<lastElement; j++)
		{
			datasetFile<<dataset[i][j]<<" ";
		}

		datasetFile<<dataset[i][lastElement]<<"\n";
	}
	datasetFile.close();
	//

	cout<<endl<<"!!!!!!! KMEANS !!!!!!" << endl;
	kmeans(dataset);
	cout<<endl<<"!!!!!!! KMEANS SIMPLER !!!!!!" << endl;
	kmeansSimple(dataset);
	cout<<endl<<"!!!!!!! CHINESE WHISPERS !!!!!!" << endl;
	chinese_whispers(dataset);
	cout<<endl<<"!!!!!!! NEWMAN CLUSTERING !!!!!!" << endl;
	newman_cluster(dataset);


	return 0;*/


	int K = 2; // number of states in one gesture
	int M = 2; // number of possible observations
	int n = 12; // length of observed sequence

	boost::shared_ptr<HMMVector<double> > pi_ptr(new HMMVector<double>(K));
	boost::shared_ptr<HMMMatrix<double> > T_ptr(new HMMMatrix<double>(K, K));
	boost::shared_ptr<HMMMatrix<double> > E_ptr(new HMMMatrix<double>(M, K));

	boost::shared_ptr<HMMVector<double> > pi2_ptr(new HMMVector<double>(K));
	boost::shared_ptr<HMMMatrix<double> > T2_ptr(new HMMMatrix<double>(K, K));
	boost::shared_ptr<HMMMatrix<double> > E2_ptr(new HMMMatrix<double>(M, K));

	HMMVector<double> &pi = *pi_ptr;
	// initial probabilities
	pi(0) = 0.5;
	pi(1) = 0.5;

	HMMMatrix<double> &T = *T_ptr;
	// transitions from state 0
	T(0, 0) = 0.5;
	T(0, 1) = 0.5;
	// transitions from state 1
	T(1, 0) = 0.5;
	T(1, 1) = 0.5;

	HMMMatrix<double> &E = *E_ptr;
	// emissions from state 0
	E(0, 0) = 0.3;
	E(1, 0) = 0.7;
	// emissions from state 1
	E(0, 1) = 0.7;
	E(1, 1) = 0.3;

	HMMVector<double> &pi2 = *pi2_ptr;
	HMMMatrix<double> &T2 = *T2_ptr;
	HMMMatrix<double> &E2 = *E2_ptr;

	//std::cout << "obs : [0, 1, 0, 1]" << std::endl;
	std::vector<sequence> testDataset;

	srand(time(0));
	for (int k = 0; k < 10; k++) {

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
		if (rand() % 10 > 10) {
			obs[round(rand() % 12)] = 1 - obs[round(rand() % 12)];
		}

		std::cout << "obs length: " << obs.size() << std::endl;
		testDataset.push_back(obs);
	}

	const int crossValK = 5;
	KFoldCrossValidation<sequence> kFoldCV(testDataset, crossValK);

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
	for (int i = 0; i < iteration_number; i++)
	{
		for (int foldNumber=0; foldNumber<kFoldCV.getK(); foldNumber++)
		{
			for (int elIndex = 0; elIndex<kFoldCV.getLearningSetSize(foldNumber); elIndex++)
			{
				sequence elValue = kFoldCV.getLearningSetElement(elIndex, foldNumber);

				double loglik = hmm->viterbi(elValue, hiddenseq);
				std::cout << "-- log likelihood of hiddenseq: " << loglik
						<< "\tLikelihood : " << exp(loglik) << std::endl;
				//std::cout << "Running forward" << std::endl;

				hmm->forward(elValue, scales, F);

				//std::cout << "Running likelihood" << std::endl;
				loglik = hmm->likelihood(scales);

				//std::cout << "Running backward" << std::endl;
				hmm->backward(elValue, scales, B);

				//std::cout << "Running posterior decoding" << std::endl;
				hmm->posterior_decoding(elValue, F, B, scales, pd);

				//std::cout << "Running Baum-Welch" << std::endl;
				hmm->baum_welch(elValue, F, B, scales, *pi2_ptr, *T2_ptr,
						*E2_ptr);

				delete hmm;
				for (int j = 0; j < K; j++) {
					pi(j) = pi2(j);

					for (int k = 0; k < M; k++) {
						E(k, j) = E2(k, j);
					}
					for (int k = 0; k < K; k++) {
						T(k, j) = T2(k, j);
					}
				}
				hmm = new HMM<double>(pi_ptr, T_ptr, E_ptr);
			}

			double total_rec = 0;
			for (int elIndex = 0; elIndex<kFoldCV.getTestingSetSize(foldNumber); elIndex++) {
				sequence elValue = kFoldCV.getTestingSetElement(elIndex, foldNumber);

				double loglik = hmm->viterbi(elValue, hiddenseq);
				total_rec += exp(loglik);
			}

			if (total_rec <= total_best)
				break;

			total_best = total_rec;
		}
	}

	std::cout << " ------ " << std::endl;
	std::cout << " Proposed model ! " << std::endl;
	std::cout << " -> Initial probabilites " << std::endl;
	std::cout << (pi)(0) << " " << (pi)(1) << std::endl;

	std::cout << " -> Transition matrix " << std::endl;
	std::cout << (T)(0, 0) << " " << (T)(0, 1) << std::endl;
	std::cout << (T)(1, 0) << " " << (T)(1, 1) << std::endl;

	std::cout << " -> Emission matrix " << std::endl;
	std::cout << (E)(0, 0) << " " << (E)(0, 1) << std::endl;
	std::cout << (E)(1, 0) << " " << (E)(1, 1) << std::endl;


}
