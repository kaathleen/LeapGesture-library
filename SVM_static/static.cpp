#include "LMReader.h"
#include <svm.h>
#include <vector>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv) {

	if(argc<3) {
		printf("Usage: %s file_class1 file_class2\n", argv[0]);
		exit(1);
	}


	// create Reader object and read given file
	LMReader *r = new LMReader(argv[1]);
	LMReader *r2 = new LMReader(argv[2]);

	vector<double> label;
	vector< vector<double> > feature;
	// if there's a frame
	int trainSetSize = 0;
	while(r->nextFrame()) 
	{
		vector<double> row;
		// get no. of fingers
		printf("%d fingers: \n", r->fingerCount);
		row.push_back(r->fingerCount);
	

		label.push_back(1);

		/*for(int i=0; i<r->fingerCount; i++) {
			row.push_back( r->getFinger(i, TIPX) );
			row.push_back( r->getFinger(i, TIPY) );
			row.push_back( r->getFinger(i, TIPZ) );

			row.push_back( r->getFinger(i, DIRX) );
			row.push_back( r->getFinger(i, DIRY) );
			row.push_back( r->getFinger(i, DIRZ) );
		}*/
		/*if(r->fingerCount != 0 )
		{
			row.push_back( r->getFinger(0, TIPX) );
			row.push_back( r->getFinger(0, TIPY) );
			row.push_back( r->getFinger(0, TIPZ) );

			//row.push_back( r->getFinger(0, DIRX) );
			//row.push_back( r->getFinger(0, DIRY) );
			//row.push_back( r->getFinger(0, DIRZ) );
		}
		else
		{
			row.push_back(0);
			row.push_back(0);
			row.push_back(0);
			//row.push_back(0);
			//row.push_back(0);
			//row.push_back(0);
		}*/


		feature.push_back(row);
		trainSetSize++;
	}

	while(r2->nextFrame()) 
	{
		vector<double> row;
		// get no. of fingers
		printf("%d fingers\n", r2->fingerCount);
		row.push_back(r2->fingerCount);
		
		
		label.push_back(2);
		// show position of fingertip
		// getfinger(FINGERID, PARAM)
		// FINGERID - id of finger in view - supports up to 20 fingers in field of view (is that enough?)
		// PARAM - available are TIPX, TIPY, TIPZ for fingertip and DIRX, DIRY, DIRZ
		for(int i=0; i<r2->fingerCount; i++) {
			
			cout<< r2->getFinger(i, DIRX)  <<" " << r2->getFinger(i, DIRY)  <<" "<<r2->getFinger(i, DIRZ)  << endl;
		}
		/*if(r2->fingerCount != 0 )
		{
			row.push_back( r2->getFinger(0, TIPX) );
			row.push_back( r2->getFinger(0, TIPY) );
			row.push_back( r2->getFinger(0, TIPZ) );

			row.push_back( r2->getFinger(0, DIRX) );
			//row.push_back( r2->getFinger(0, DIRY) );
			//row.push_back( r2->getFinger(0, DIRZ) );
		}
		else
		{
			row.push_back(0);
			row.push_back(0);
			row.push_back(0);
			//row.push_back(0);
			//row.push_back(0);
			//row.push_back(0);
		}*/


		feature.push_back(row);
		trainSetSize++;
	}




	// Defining training set
	svm_problem *problem = new svm_problem;
 	memset(problem,0,sizeof(svm_problem));
	
	problem->l = trainSetSize;
	int featureSize = feature[0].size();	

	//dynamically allocate the trainSetSize * featureSize matrix
    problem->x = new svm_node*[trainSetSize];
    for(int row = 0; row < trainSetSize; ++row)
        problem->x[row] = new svm_node[featureSize + 1];

	// dynamically allocate the labels
    problem->y = new double[trainSetSize];

	// filling main matrix
    for(int row = 0; row < trainSetSize; ++row)
    {
        for(int col = 0; col < featureSize; ++col)
        {
            problem->x[row][col].index = col + 1;
            problem->x[row][col].value = feature[row][col];
        }

		// ending the sequence of features
        problem->x[row][feature[row].size()].index = -1;
        problem->x[row][feature[row].size()].value = 0;

        // defining the label
        problem->y[row] = label[row];
    }

	// Setting parameters
 	svm_parameter *svmParameter = new svm_parameter;
    svmParameter->svm_type = C_SVC;
    svmParameter->kernel_type = RBF;
    svmParameter->nr_weight = 0;
    svmParameter->eps = 0.001;
    svmParameter->degree = 1;
    svmParameter->shrinking = 0;
    svmParameter->probability = 1;
    svmParameter->cache_size = 100; 
	// parameters (C, gamma) to find by the cross validation	
	svmParameter->C = 1000000.0;
    svmParameter->gamma = 0.0001; 


	/// Looking for model
	cout<<"---> Model calculation"<<endl;	
	svm_model *svmModel = svm_train(problem, svmParameter);

	/// Prediction on the training set
	cout<<"---> Predicition!"<<endl;
	int count = 0;
	for ( int j=0;j< trainSetSize ;j++)
	{
		svm_node *node = new svm_node[ featureSize + 1];
		for (int k=0; k< featureSize ;k++)
		{
			node[k].index = k+1;
			node[k].value = feature[j][k];
		}

		node[featureSize].index = -1;

		// Calcualting probabilities
		double prob_est[2];
		svm_predict_probability(svmModel, node, prob_est);
		
		float pred = 1;
		if ( prob_est[0] < prob_est[1] )
		{
			pred = 2;
		}

		// This returns the label of the class with the highest probability
		//float pred = (float)svm_predict(svmModel, node);

		// If the classification was correct
		if ( pred == label[j] )
		{
			count++;
		}
	}
	cout<<"Prediction : "<< (count*1.0 / trainSetSize * 100.0) << "%"<<endl;
	cout<<"OK : " << count << endl;
	cout<<"Total : " << trainSetSize << endl;

	return 0;
}



