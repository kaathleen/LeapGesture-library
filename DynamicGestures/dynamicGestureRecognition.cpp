#include <HMMlib/hmm_table.hpp>
#include <HMMlib/hmm_vector.hpp>
#include <HMMlib/hmm.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>

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

#include "preprocess/LMpre.h"

#include "HMM/HMMClass.h"

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
void kmeansSimple(std::vector<std::vector<double> > *data, int count,
		vector< vector<double> > &centroids, int classCounter = 13) {

	typedef dlib::matrix<double> sample_type;

	std::vector<sample_type> samples;
	std::vector<sample_type> initial_centers;

	// Copying to compatible type
	for (int k=0;k<count;k++)
	{
		for (int i = 0; i < data[k].size(); i++) {
			sample_type m;
			m.set_size(data[k][i].size(),1);

			for (int j = 0; j < data[k][i].size(); j++) {
				m(j) = data[k][i][j];
			}
			samples.push_back(m);
		}
	}

	typedef dlib::radial_basis_kernel<sample_type> kernel_type;
	pick_initial_centers(classCounter, initial_centers, samples, kernel_type(0.1));
	dlib::find_clusters_using_kmeans(samples, initial_centers);

	ofstream kmeanscentroids;
	kmeanscentroids.open("centroids.dat");
	for (int i = 0; i < initial_centers.size(); i++) {
		kmeanscentroids << i <<" " << trans(initial_centers[i]);
		centroids.push_back(vector< double >(initial_centers[i].size()));
		for (int j = 0; j< initial_centers[i].size(); j++)
		{
		//	cout<<initial_centers[i].size()<<endl;
		//	cout<<initial_centers[i](j)<<endl;
			centroids[i][j] = initial_centers[i](j);
		}

	}
	kmeanscentroids.close();
}

void readCentroidsFromFile( vector< vector<double> > &centroids )
{
	ifstream kmeanscentroids;
	kmeanscentroids.open("centroids.dat");
	string line;
	double val;
	while(!kmeanscentroids.eof()) {
		kmeanscentroids>>val;
		if ( val != 0 )
		{
			vector<double> row;
			for (int i=0;i<19;i++)
			{
				kmeanscentroids>>val;
				row.push_back(val);
			}
			centroids.push_back(row);
		}
	}
	kmeanscentroids.close();
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
				float angle = fingerDirection.dotProduct(fingerDirection);
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

void anglesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {

		vector<float> angles(3, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex fingerDirection = tempHand->getFinger(i)->getDirection();
			float angle = fingerDirection.dotProduct(tempHand->getPalmNormal());
			angles.push_back(angle);	// Can be acos if somebody want it
		}

		sort(angles.begin(), angles.end(), std::greater<float>());

		for (int i = 0; i < 6; i++) {
			addAttribute(angles[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else {
		for (int i = 0; i < 6; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}

void distancesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {


		vector<float> distances(4, 0.0);

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

		for (int i = 0; i < 4; i++) {
			addAttribute(distances[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 4; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}


void handMovementAttribute(GestureHand* tempHand, GestureHand* tempHand2,
		int& attributeCounter, vector<double>& result) {

	Vertex palmPos = tempHand->getPalmPosition();
	Vertex palmPos2 = tempHand2->getPalmPosition();

	addAttribute(palmPos2.getX() - palmPos.getX(), attributeCounter, result);
	addAttribute(palmPos2.getY() - palmPos.getY(), attributeCounter, result);
	addAttribute(palmPos2.getZ() - palmPos.getZ(), attributeCounter, result);
}

// Computes the feature set
vector<double> computeFeatureSet(GestureFrame *gestureFrame, GestureFrame *gestureFrame2) {
	vector<double> result;

	// Get the numbe of fingers
	GestureHand *tempHand = gestureFrame->getHand(0);
	GestureHand *tempHand2 = gestureFrame2->getHand(0);
	int fingerCount =
			(tempHand != NULL) ?
					min(tempHand->getFingerCount(), MAX_FINGER_COUNT) : 0;
	int attributeCounter = 1;

	// Adding the finger count to the feature set
	fingerCountAttribute(fingerCount, attributeCounter, result);

	int fingerCount2 =
				(tempHand != NULL) ?
						min(tempHand2->getFingerCount(), MAX_FINGER_COUNT) : 0;

	// Adding the finger count to the feature set
	fingerCountAttribute(fingerCount2, attributeCounter, result);

	// Adding 3 angles to the palm normal
	//anglesFingersPalmAttribute(tempHand, fingerCount, attributeCounter,
	//			result);

	// Adding the 6 highest angles to the feature set
	//anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
	//		result);

	// Adding the 6 greatest distances to the feature set
	//distancesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
	//		result);
	distancesBetweenFingersAttribute(tempHand2, fingerCount2, attributeCounter,
				result);


	// Hand movement
	handMovementAttribute(tempHand, tempHand2, attributeCounter, result);

	return result;
}

void columnScaling(vector< vector<double > >& data ) {

	int trainSetSize = data.size();
	int featureSize = data[0].size();
	double ** scaling = new double*[2];
	scaling[0] = new double [ featureSize ];
	scaling[1] = new double [ featureSize ];

	for (int j = 0; j < featureSize; j++) {
		double min_val = data[0][j], max_val = data[0][j];
		for (int i = 0; i < trainSetSize; i++) {
			if (data[i][j] < min_val) {
				min_val = data[i][j];
			}
			if (data[i][j] > max_val) {
				max_val = data[i][j];
			}
		}
		scaling[0][j] = min_val;
		scaling[1][j] = max_val - min_val;

		if ( scaling[1][j] > 0.1)
		{
			for (int i = 0; i < trainSetSize; i++) {
				data[i][j] = (data[i][j] - scaling[0][j]) / (scaling[1][j]);
			}
		}
	}
}

void readingInputData(string fileName, vector<GestureFrame> &frames) {
	GestureStorageDriver* gestureStorageDriver = new BinaryFileStorageDriver();

	gestureStorageDriver->openConnection(fileName, false);

	GestureFrame currGestureFrame;
	while (gestureStorageDriver->loadGestureFrame(currGestureFrame)) {
		frames.push_back(currGestureFrame);
		currGestureFrame.clear();
	}
	gestureStorageDriver->closeConnection();
	delete gestureStorageDriver;
}

void dataPreparation(int count, vector<GestureFrame> *frames,
		const int preprocessingWidth,
		vector<vector<double> > *dataset) {
	for (int i = 0; i < count; i++) {

		LMpre::LMpre pre(frames[i], preprocessingWidth);
		frames[i] = pre.process();
		for (int j = 0; j < frames[i].size(); j++) {
			vector<double> row;
			int k = (j + 10) < frames[i].size() ? (j+10) : (frames[i].size()-1);

			row = computeFeatureSet(&frames[i][j], &frames[i][k]);
			dataset[i].push_back(row);
		}
		// Data normalizaion in columns
		columnScaling(dataset[i]);
	}
}

//void saveToEstimateNumbeOfClasses(vector<vector<double> > *dataset, int argc) {
//	// Saving for tool
//	ofstream zapis("out.csv");
//	for (int i = 0; i < argc -1 ; i++) {
//		if (i == 0) {
//			for (int j = 0; j < dataset[i].size(); j++) {
//				zapis << '"' << j << '"' << ';';
//			}
//			zapis << endl;
//		}
//		zapis << '"' << i << '"' << ";";
//		for (int j = 0; j < dataset[i].size(); j++) {
//			zapis << dataset[i][j] << ";";
//		}
//		zapis << endl;
//	}
//	zapis.close();
//
//	std::fstream datasetFile;
//	datasetFile.open("DATASET_FILE", std::fstream::out | std::fstream::trunc);
//	for (int i = 0; i < argc -1; i++) {
//		int lastElement = dataset[i].size() - 1;
//		for (int j = 0; j < lastElement; j++) {
//			datasetFile << dataset[i][j] << " ";
//		}
//
//		datasetFile << dataset[i][lastElement] << "\n";
//	}
//	datasetFile.close();
//}

void readConfig(int &preprocessingWidth, int &crossValK, int &K, int &M,
		int &classNumber, int &readCentroids, double &learningRate,
		int &iteration_number, int &datasetSize)
{
	ifstream in;
	in.open("config.cfg");

	string tmp;
	string val;

	getline(in, tmp);
	getline(in, val);
	preprocessingWidth = atoi(val.c_str());

	getline(in, tmp);
	getline(in, val);
	crossValK = atoi(val.c_str());

	getline(in, tmp);
	getline(in, val);
	K = atoi(val.c_str());

	getline(in, tmp);
	getline(in, val);
	classNumber = atoi(val.c_str());
	M = classNumber;

	getline(in, tmp);
	getline(in, val);
	readCentroids = atoi(val.c_str());

	getline(in, tmp);
	getline(in, val);
	learningRate = atof(val.c_str());

	getline(in, tmp);
	getline(in, val);
	iteration_number = atoi(val.c_str());

	getline(in, tmp);
	getline(in, val);
	datasetSize = atoi(val.c_str());

	in.close();
}

int main(int argc, char **argv) {
	// Parameters to play with
	int preprocessingWidth = 10;
	int crossValK = 5;
	int K = 10; // number of states in one gesture
	int M = 15;
	int classNumber = 15; // number of possible observations
	int readCentroids = 1; // 0 - calculate using k-means, 1 - read them from file
	double learningRate = 0.2; // How much do we incorporate new training data into trained model
	int iteration_number = 100; // How many learning iterations
	int datasetSize = 180;

	// Read parameters
	readConfig(preprocessingWidth, crossValK, K, M, classNumber, readCentroids,
			learningRate, iteration_number, datasetSize);

	// Reading data in
	vector<GestureFrame> frames[180];
	vector<int> label;

	for (int k=0;k<6;k++)
	{
		string base = "dataset/";
		if ( k == 0)
			base = base + "123/";
		else if (k == 1)
			base = base + "drzwi/";
		else if (k == 2)
			base = base + "kolko/";
		else if (k == 3)
			base = base + "nozyce/";
		else if (k == 4)
			base = base + "pistolet/";
		else
			base = base + "przenoszenie/";
		for (int j=1;j<31;j++)
		{
			string name = base;
			stringstream ss2;
			ss2 << j;
			name = name + ss2.str() + ".lmr";
			readingInputData(name, frames[k*30 + j-1 ]);
			label.push_back(k);
		}
	}
	cout<<"Ended loading data" <<endl;

	// Preprocessing &&
	// Saving all possible gestures as feature sets &&
	// Scaling
	vector<vector<double> > dataset[datasetSize];
	dataPreparation(datasetSize, frames, preprocessingWidth, dataset);

	// Saving for tool to estimate number of classes
	//saveToEstimateNumbeOfClasses(dataset, argc);

	cout << " k-means " << endl;
	vector< vector<double> > centroids;
	if ( readCentroids )
		readCentroidsFromFile(centroids);
	else
		kmeansSimple(dataset, datasetSize, centroids, classNumber);

	//cout<<endl<<"!!!!!!! KMEANS !!!!!!" << endl;
	//kmeans(dataset, 11);
	//cout<<endl<<"!!!!!!! CHINESE WHISPERS !!!!!!" << endl;
	///chinese_whispers(dataset);
	//cout<<endl<<"!!!!!!! NEWMAN CLUSTERING !!!!!!" << endl;
	//newman_cluster(dataset);

	// Seperating into train/test dataset
	std::vector<sequence> trainDataset[6];
	std::vector<sequence> testDataset;

	// Finding the longest sequence of observations
	int n = 0;
	for (int p=0;p<datasetSize;p++)
	{
		n = max(n, (int)dataset[p].size());
	}

	cout <<"Seperating data"<<endl;
	// Determine the observation values for all sequences
	for (int p=0;p<datasetSize;p++)
	{
		vector<unsigned int> observationLabels(n, 0.0);
		for (int j=0; j <  n;j++)
		{
			// Fill the rest with the last label
			if ( j >= dataset[p].size() )
			{
				observationLabels[j] = observationLabels[dataset[p].size() - 1];
			}
			else
			{
				// Calculate the errors to the centroids
				double best_error = 0.0;
				int best_index = -2;
				for (int i = 0; i < centroids.size(); i++) {
					double error = 0.0;
					for (int k = 0; k < centroids[i].size(); k++) {
						error += (dataset[p][j][k] - centroids[i][k])
								* (dataset[p][j][k] - centroids[i][k]);
					}
					if (best_error > error || best_index < 0) {
						best_error = error;
						best_index = i;
					}
				}
				observationLabels[j] = (best_index);
			}
		}
		// First 20 used for training
		if ( p % 30 < 20)
			trainDataset[p/30].push_back(observationLabels);

		testDataset.push_back(observationLabels);
	}


	cout<<"HMM start"<<endl;

	// HMM
	HMMClass *hmmGesture[6];
	for (int i=0;i<6;i++)
	{
		cout<<"Learning model i = " << i << " on " <<trainDataset[i].size()<< " samples" <<endl;
		hmmGesture[i] = new HMMClass(K, n, M);
		hmmGesture[i]->train(trainDataset[i],crossValK, iteration_number, learningRate);
		cout<<"Model learnt" << endl;
	}

	//HMMClass *hmmGesture = new HMMClass("hmmFirstModel.model");

	vector<int> predictedLabels;
	for (int i=0;i<datasetSize;i++)
	{
		double error;
		int index = -1;
		for (int j=0;j<6;j++)
		{
			double loglik = hmmGesture[j]->predict(testDataset[i]);
			if ( index == -1 || loglik > error)
			{
				error = loglik;
				index = j;
			}
		}
		predictedLabels.push_back(index);
	}

	cout<<"Counting percentage ..."<<endl;
	int counter = 0;
	for (int i=0; i< datasetSize;i++)
		if ( predictedLabels[i] == label[i])
			counter++;

	cout<<"Total recognition rate : " << counter * 100.0 / datasetSize << endl;

	//hmmGesture->show();



	for (int j=0;j<6;j++)
	{
		string name = "hmmModel_";
		stringstream ss2;
		ss2 << (j+1);
		name = name + ss2.str() + ".model";
		hmmGesture[j]->saveModel(name);
		delete hmmGesture[j];
	}
}
