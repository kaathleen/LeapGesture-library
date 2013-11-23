#include "LMReader.h"

#include <vector>
#include "SVMclassificator.h"

using namespace std;

int main(int argc, char **argv) {

	// This is optimal. Change only in special cases
	const int TRAIN_TEST_PERCENT = 50;

	if(argc<3) {
		printf("Usage: %s file_class1 file_class2 ...\n", argv[0]);
		exit(1);
	}
	const int NUMBER_OF_CLASSES = argc - 1;

	// create Reader object and read given file
	LMReader *r[NUMBER_OF_CLASSES];
	for (int i=0;i<NUMBER_OF_CLASSES;i++)
	{
		r[i] = new LMReader(argv[i+1]);
	}
	// For every class we have a matrix:
	//  - in which each row is a row of features for 1 example
	//  - number of rows = number of samples
	vector< vector<double> > feature[NUMBER_OF_CLASSES];

	// storing the size of each class
	int setSize[NUMBER_OF_CLASSES];
	memset(setSize, 0, sizeof(int) * NUMBER_OF_CLASSES);

	for (int j=0;j<NUMBER_OF_CLASSES;j++)
	{
		while(r[j]->nextFrame())
		{
			// Defining the set of features
			vector<double> row;

			// 1 feature -> number of fingers
			row.push_back(r[j]->fingerCount);

			// !!!
			// DEFINE YOUR OWN FEATURES HERE !
			// remember that each feature row should be equal size --> this can be changed but do we need it?
			// !!!

			// PARAM - available are TIPX, TIPY, TIPZ for fingertip and DIRX, DIRY, DIRZ
			/*for(int i=0; i<r->fingerCount; i++) {
				row.push_back( r->getFinger(i, TIPX) );
				row.push_back( r->getFinger(i, TIPY) );
				row.push_back( r->getFinger(i, TIPZ) );

				row.push_back( r->getFinger(i, DIRX) );
				row.push_back( r->getFinger(i, DIRY) );
				row.push_back( r->getFinger(i, DIRZ) );
			}*/

			// Adding it to the set
			feature[j].push_back(row);
			setSize[j]++;
		}
	}
	
	// Separation of data into learning and testing sets
	int trainSetSize = 0;
	int testSetSize = 0;

	vector<int> trainLabel, testLabel;
	vector< vector<double> > trainFeatures, testFeatures;
	for (int i=0;i<NUMBER_OF_CLASSES;i++)
	{
		int class_train_size = (setSize[i] * TRAIN_TEST_PERCENT)/100;
		int class_test_size = setSize[i] - class_train_size;
		trainSetSize+= class_train_size;
		testSetSize+= class_test_size;

		for (int j=0; j< class_train_size ; j++)
		{
			trainLabel.push_back(i+1);
			trainFeatures.push_back(feature[i][j]);
		}
		for (int j= class_train_size; j<setSize[i] ; j++)
		{
			testLabel.push_back(i+1);
			testFeatures.push_back(feature[i][j]);
		}
	}


	// Creating an object responsible for learning
	SVMclassificator SVM;

	// Learning the classifier
	SVM.train(trainFeatures, trainLabel, NUMBER_OF_CLASSES);

	// Predicting the labels on the other TRAIN_TEST_PERCENT
	vector<int> pred = SVM.classify(testFeatures);

	// Counting the number of correctly recognized labels
	int count = 0;
	for (int i=0;i<testSetSize; i++)
	{
		if ( pred[i] == testLabel[i] )
		{
			count++;
		}
	}

	// Printing results
	cout<<endl<<"-------- Result --------" << endl;
	cout<<"Recognition rate [%] : "<< (count*1.0 / testSetSize * 100.0) << "%"<<endl;
	cout<<"Correctly recognized [number] : " << count << endl;
	cout<<"Total size of test set [number] : " << testSetSize << endl;

	return 0;
}



