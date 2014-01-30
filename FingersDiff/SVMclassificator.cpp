#include "SVMclassificator.h"

using namespace std;

// Data scaling to interval [LOWER_RANGE, UPPER_RANGE]
void SVMclassificator::dataScaling(vector<vector<double> >& trainDataset) {
	scaling = new double*[2];
	scaling[0] = new double[featureSize];
	scaling[1] = new double[featureSize];
	
	int LOWER_RANGE = -1;
	int UPPER_RANGE = 1;
	int NEUTRAL_VALUE = 0;

	for (int j = 0; j < featureSize; j++) {
		double min_val = trainDataset[0][j], max_val = trainDataset[0][j];
		for (unsigned int i = 0; i < trainDataset.size(); i++) {
			if (trainDataset[i][j] < min_val) {
				min_val = trainDataset[i][j];
			}
			if (trainDataset[i][j] > max_val) {
				max_val = trainDataset[i][j];
			}
		}
		scaling[0][j] = min_val;
		scaling[1][j] = max_val;

		for (unsigned int i = 0; i < trainDataset.size(); i++) {
			if (max_val - min_val > 0) {
				trainDataset[i][j] = (trainDataset[i][j] - min_val)
						* (UPPER_RANGE - LOWER_RANGE) / (max_val - min_val)
						+ LOWER_RANGE;
			}
			else {
				trainDataset[i][j] = NEUTRAL_VALUE;
			}
		}
	}
}

// Data scaling to interval [0.0 , 1.0]
void SVMclassificator::dataScalingClassify(vector<vector<double> >& data) {

	int trainSetSize = data.size();

	for (int j = 0; j < featureSize; j++) {
		for (int i = 0; i < trainSetSize; i++) {
			data[i][j] = (data[i][j] - scaling[0][j]) / (scaling[1][j]);
		}
	}
}

void SVMclassificator::train(vector< vector<double> > data, vector<int> label, const int _NUMBER_OF_CLASSES) {
	NUMBER_OF_CLASSES = _NUMBER_OF_CLASSES;
	int trainSetSize = label.size();
	featureSize = data[0].size();

	// Data scaling to interval [0.0 , 1.0]
	dataScaling(data);
	printf("Data scaled properly\n");

	// Defining training set
	svmTrainingSet = new svm_problem;
	memset(svmTrainingSet, 0, sizeof(svm_problem));

	svmTrainingSet->l = trainSetSize;

	//dynamically allocate the trainSetSize * featureSize matrix
	svmTrainingSet->x = new svm_node*[trainSetSize];
	for (int row = 0; row < trainSetSize; ++row)
		svmTrainingSet->x[row] = new svm_node[featureSize + 1];

	// dynamically allocate the labels
	svmTrainingSet->y = new double[trainSetSize];

	// filling main matrix
	for (int row = 0; row < trainSetSize; ++row) {
		for (int col = 0; col < featureSize; ++col) {
			svmTrainingSet->x[row][col].index = col + 1;
			svmTrainingSet->x[row][col].value = data[row][col];
		}

		// ending the sequence of features
		svmTrainingSet->x[row][featureSize].index = -1;
		svmTrainingSet->x[row][featureSize].value = 0;

		// defining the label
		svmTrainingSet->y[row] = label[row];
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


	ofstream cv;
	cv.open("CrossValidation.tmp");
	double best_c = -5, best_g = -15, best_cross = 0;
	/// Looking for model
	for (double c = -5; c < 15; c++) {
		for (double gamma = -15; gamma < 3; gamma++) {
			svmParameter->C = pow(2, c);
			svmParameter->gamma = pow(2, gamma);


			svm_model *svmModel = svm_train(svmTrainingSet, svmParameter);

			double target[trainSetSize];
			svm_cross_validation(svmTrainingSet, svmParameter, 5, target);

			/*int count = 0;
			for (int j = 0; j < trainSetSize; j++) {
				if (target[j] == label[j]) {
					count++;
				}
			}
			double cross = (count * 1.0 / trainSetSize * 100.0);*/
			
			int correct = 0;
			for(int i=0; i<svmTrainingSet->l; i++)
			{
				if(target[i] == svmTrainingSet->y[i])
				{
					correct++;
				}
			}
			double cross = 100.0*(correct / svmTrainingSet->l);

			cv<<"C: "<<c<<" gamma: "<<gamma<<" Recognition: "<<cross<<"%"<<std::endl;
			cout<<"-----------------------------------------------------------------"<<endl;
			cout<<"C: "<<c<<" gamma: "<<gamma<<" Recognition: "<<cross<<"%"<<std::endl;
			cout<<"-----------------------------------------------------------------"<<endl;

			if (cross > best_cross) {
				best_cross = cross;
				best_g = gamma;
				best_c = c;
			}
		}
	}
	
	cv<<std::endl<<"--------------BEST!-------------"<<std::endl<<"C: "<<best_c<<" gamma: "<<best_g<<" Recognition: "<<best_cross<<"%"<<std::endl;
	cv.close();

	// Using best model
	svmParameter->C = pow(2, best_c);
	svmParameter->gamma = pow(2, best_g);


	svmModel = svm_train(svmTrainingSet, svmParameter);
}

std::vector<int> SVMclassificator::classify(
		std::vector<std::vector<double> > data) {
	int testSetSize = data.size();

	// Data scaling to interval [0.0 , 1.0]
	dataScalingClassify(data);

	vector<int> classifiedLabels;
	int count = 0;
	for (int j = 0; j < testSetSize; j++) {
		svm_node *node = new svm_node[featureSize + 1];
		for (int k = 0; k < featureSize; k++) {
			node[k].index = k + 1;
			node[k].value = data[j][k];
		}

		node[featureSize].index = -1;

		// Calculating probabilities
		double prob_est[NUMBER_OF_CLASSES];
		svm_predict_probability(svmModel, node, prob_est);

		float pred = -1, best_prob = -1;
		for (int k=0;k<NUMBER_OF_CLASSES;k++)
		{
			if ( prob_est[k] > best_prob)
			{
				best_prob = prob_est[k];
				pred = k+1;
			}
		}

		// This returns the label of the class with the highest probability
		//float pred = (float)svm_predict(svmModel, node);

		// If the classification was correct
		classifiedLabels.push_back(pred);
	}
	return classifiedLabels;
}
