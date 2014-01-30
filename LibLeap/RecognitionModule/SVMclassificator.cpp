#include "SVMclassificator.h"

using namespace std;

const int SVMclassificator::TRAIN_LOWER_RANGE = -1;
const int SVMclassificator::TRAIN_UPPER_RANGE = 1;

SVMclassificator::SVMclassificator() {
	logger = new LogUtil("SVM classificator");
}

SVMclassificator::~SVMclassificator() {
	delete logger;
}

TrainingResult* SVMclassificator::train(
		std::vector<std::vector<double> >& trainDataset,
		std::vector<int>& trainLabels, int classNumber,
		std::string configurationPath, std::string configurationName,
		bool saveScaleFile, int kCrossValParam) {
	logger->debug("train entry");

	this->trainConfPath = configurationPath;
	this->trainConfName = configurationName;
	this->saveScaleFile = saveScaleFile;

	// Data scaling to interval [LOWER_RANGE, UPPER_RANGE]
	dataScalingTrain(trainDataset, trainLabels);
	logger->debug("data was scaled");

	// Defining training set
	this->svmTrainingSet = defineSvmTrainingSet(trainDataset, trainLabels);
	logger->debug("training set was defined");

	// Setting parameters
	svm_parameter *svmParameter = setTrainingParameters();
	logger->debug("parameters was set");

	/// Search learning parameters
	TrainingResult *trainResult = new TrainingResult();
	double best_c = -5, best_g = -15, best_cross = 0;
	searchLearningParameters(svmParameter, trainLabels, classNumber,
			kCrossValParam, best_cross, best_g, best_c, trainResult);
	logger->debug("learning parameters was searched");

	// Using best model
	svmParameter->C = pow(2, best_c);
	svmParameter->gamma = pow(2, best_g);
	svm_model *svmModel = svm_train(svmTrainingSet, svmParameter);
	logger->debug("train model was defined");

	// Save model
	std::string modelFilePath = PathUtil::combinePathFileNameAndExt(
			configurationPath, configurationName, ConfExt::MODEL_EXT);
	svm_save_model(modelFilePath.c_str(), svmModel);
	logger->debug("train model was saved");

	logger->info("train exit");

	return trainResult;
}

TestingResult* SVMclassificator::classify(
		std::vector<double>& testDataset, int classNumber, std::string configurationPath,
		std::string configurationName, double classificationThresholdRate) {
	logger->debug("classify entry");

	this->testConfPath = configurationPath;
	this->testConfName = configurationName;

	int featureSize = testDataset.size();

	// Data scaling to interval [0.0 , 1.0]
	dataScalingClassify(testDataset);

	// Load model
	std::string modelFilePath = PathUtil::combinePathFileNameAndExt(
			testConfPath, testConfName, ConfExt::MODEL_EXT);
	svm_model *svmModel = svm_load_model(modelFilePath.c_str());

	TestingResult* classificationResult = new TestingResult();
	svm_node *node = new svm_node[featureSize + 1];
	for (int k = 0; k < featureSize; k++) {
		node[k].index = k + 1;
		node[k].value = testDataset[k];
	}
	node[featureSize].index = -1;

	// Calculating probabilities
	double prob_est[classNumber];

	svm_predict_probability(svmModel, node, prob_est);

	float pred = -1, best_prob = -1;
	for (int k = 0; k < classNumber; k++) {
		TestingClassResult classResult = TestingClassResult();
		classResult.classTrainRate = prob_est[k];
		classResult.genericClassName = k;
		classificationResult->testClassResults.push_back(classResult);

		logger->sdebug()<<"classTrainRate: "<<prob_est[k]<<endl;
		logger->sdebug()<<"k: "<<k<<endl;

		if (prob_est[k] > best_prob) {
			best_prob = prob_est[k];
			pred = k;
		}
	}
	if (best_prob >= classificationThresholdRate) {
		classificationResult->recognized = true;
		classificationResult->genericClassName = pred;
	} else {
		classificationResult->recognized = false;
	}

	logger->debug("classify exit");

	return classificationResult;
}

// Data scaling to interval [LOWER_RANGE, UPPER_RANGE]
void SVMclassificator::dataScalingTrain(vector<vector<double> >& trainDataset,
		std::vector<int>& trainLabels) {
	logger->debug("dataScaling entry");

	int featureSize = trainDataset[0].size();
	double **scaling = new double*[2];
	scaling[0] = new double[featureSize];
	scaling[1] = new double[featureSize];

	bool* resetFeatures = new bool[featureSize];

	for (int j = 0; j < featureSize; j++) {
		resetFeatures[j] = false;

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
			trainDataset[i][j] = scaleValue(trainDataset[i][j], scaling[0][j],
					scaling[1][j], TRAIN_LOWER_RANGE, TRAIN_UPPER_RANGE);
			if (max_val <= min_val)
				resetFeatures[j] = true;

			logger->sdebug() << "trainDataset[" << i << "][" << j << "] = "
					<< trainDataset[i][j] << std::endl;
		}
	}

	saveScaleAndRangeFiles(trainDataset, trainLabels, scaling, resetFeatures);

	delete[] resetFeatures;

	logger->debug("dataScaling exit");
}

void SVMclassificator::dataScalingClassify(
		vector<double>& testDataset) {
	logger->debug("dataScalingClassify entry");

	int featureSize = testDataset.size();
	double** scaling = loadRangeFile(featureSize);

	for (int j = 0; j < featureSize; j++) {
			testDataset[j] = scaleValue(testDataset[j], scaling[0][j],
					scaling[1][j], testLowerRange, testUpperRange);
	}

	logger->debug("dataScalingClassify exit");
}

double SVMclassificator::scaleValue(double value, double minValue,
		double maxValue, int lowerRange, int upperRange) {
	if (maxValue <= minValue)
		return 0;

	value = max(value, minValue);
	value = min(value, maxValue);

	double result = (value - minValue) * (upperRange - lowerRange)
			/ (maxValue - minValue) + lowerRange;

	return result;
}

svm_problem* SVMclassificator::defineSvmTrainingSet(
		std::vector<std::vector<double> >& trainDataset,
		std::vector<int>& trainLabels) {
	logger->debug("defineSvmTrainingSet entry");

	int featureSize = trainDataset[0].size();
	svm_problem* svm_prob = new svm_problem;
	memset(svm_prob, 0, sizeof(svm_problem));

	int trainSetSize = trainLabels.size();
	svm_prob->l = trainSetSize;

	//dynamically allocate the trainSetSize * featureSize matrix
	svm_prob->x = new svm_node*[trainSetSize];
	for (int row = 0; row < trainSetSize; ++row)
		svm_prob->x[row] = new svm_node[featureSize + 1];

	// dynamically allocate the labels
	svm_prob->y = new double[trainSetSize];

	// filling main matrix
	for (int row = 0; row < trainSetSize; ++row) {
		for (int col = 0; col < featureSize; ++col) {
			svm_prob->x[row][col].index = col + 1;
			svm_prob->x[row][col].value = trainDataset[row][col];
		}

		// ending the sequence of features
		svm_prob->x[row][featureSize].index = -1;
		svm_prob->x[row][featureSize].value = 0;

		// defining the label
		svm_prob->y[row] = trainLabels[row];
	}

	logger->debug("defineSvmTrainingSet exit");

	return svm_prob;
}

svm_parameter* SVMclassificator::setTrainingParameters() {
	logger->debug("setTrainingParameters entry");

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

	logger->debug("setTrainingParameters exit");

	return svmParameter;
}

void SVMclassificator::searchLearningParameters(svm_parameter*& svmParameter,
		std::vector<int>& trainLabels, int classNumber, int kCrossValParam,
		double best_cross, double& best_g, double& best_c,
		TrainingResult* trainResult) {
	logger->debug("searchParameters entry");

	for (double c = -5; c < 15; c++) {
		for (double gamma = -15; gamma < 3; gamma++) {
			svmParameter->C = pow(2, c);
			svmParameter->gamma = pow(2, gamma);
			double target[trainLabels.size()];
			svm_cross_validation(svmTrainingSet, svmParameter, kCrossValParam,
					target);

			int correct = 0;
			int classesCorrect[classNumber];
			int classesAll[classNumber];
			for (int i = 0; i < classNumber; i++) {
				classesCorrect[i] = 0;
				classesAll[i] = 0;
			}
			for (int i = 0; i < svmTrainingSet->l; i++) {
				classesAll[((int) target[i])]++;
				if (target[i] == svmTrainingSet->y[i]) {
					classesCorrect[((int) target[i])]++;
					correct++;
				}
			}

			double cross = 0;
			if (svmTrainingSet->l != 0)
				cross = correct / svmTrainingSet->l;

			logger->sinfo() << "C: " << c << " gamma: " << gamma
					<< " Recognition: " << 100.0 * cross << "%" << std::endl;
			if (cross > best_cross) {
				best_cross = cross;
				best_g = gamma;
				best_c = c;
				trainResult->trainRate = best_cross;
				trainResult->trainClassResults.clear();
				for (int j = 0; j < classNumber; j++) {
					double classCross = 0;
					if (classesAll[j] != 0)
						classCross = classesCorrect[j] / classesAll[j];

					trainResult->trainClassResults.push_back(
							TrainingClassResult(j, classCross));
				}
			}
		}
	}

	logger->debug("searchParameters exit");
}

void SVMclassificator::saveScaleAndRangeFiles(
		std::vector<std::vector<double> >& trainDataset,
		std::vector<int>& trainLabels, double** scaling, bool* resetFeatures) {
	logger->debug("saveScaleAndRangeFiles entry");

	int featureSize = trainDataset[0].size();

	// save scale file
	std::string scaleFilePath = PathUtil::combinePathFileNameAndExt(
			trainConfPath, trainConfName, ConfExt::SCALE_EXT);
	FileWriterUtil scaleFile(scaleFilePath, !saveScaleFile);
	scaleFile.open();
	for (unsigned int i = 0; i < trainDataset.size(); i++) {
		scaleFile << trainLabels[i] << " ";
		for (int j = 0; j < featureSize; j++) {
			if (!resetFeatures[j])
				scaleFile << j + 1 << ":" << trainDataset[i][j] << " ";
		}
		scaleFile << "\n";
	}
	scaleFile.close();

	// save range file
	std::string rangeFilePath = PathUtil::combinePathFileNameAndExt(
			trainConfPath, trainConfName, ConfExt::RANGE_EXT);
	FileWriterUtil rangeFile(rangeFilePath);
	rangeFile.open();
	rangeFile << "x\n" << TRAIN_LOWER_RANGE << " " << TRAIN_UPPER_RANGE << "\n";
	for (int j = 0; j < featureSize; j++) {
		if (scaling[0][j] != scaling[1][j]) {
			rangeFile << j + 1 << " " << scaling[0][j] << " " << scaling[1][j]
					<< "\n";
		}
	}
	rangeFile.close();

	logger->debug("saveScaleAndRangeFiles exit");
}

double** SVMclassificator::loadRangeFile(int featureSize) {
	enum LoadRangeFileStep {
		XSCALING_STEP, RANGES_STEP, FEATURES_RANGES_STEP
	};

	logger->debug("loadRangeFile entry");

	std::string rangeFilePath = PathUtil::combinePathFileNameAndExt(
			testConfPath, testConfName, ConfExt::RANGE_EXT);
	std::ifstream rangeFile;
	rangeFile.open(rangeFilePath.c_str(), std::fstream::in);

	if (!rangeFile.is_open()) {
		// throw exception
	}

	LoadRangeFileStep currStep = XSCALING_STEP;
	std::vector<FeatureRange> classRanges;
	std::string line;
	while (std::getline(rangeFile, line)) {
		logger->sdebug()<<"LINE: "<<line<<std::endl;
		std::istringstream iss(line);

		if (currStep == XSCALING_STEP) {
			logger->sdebug()<<"XSCALING_STEP"<<std::endl;
			if (line == "x") {
				currStep = RANGES_STEP;
			}
		}
		else if (currStep == RANGES_STEP) {
			logger->sdebug()<<"RANGES_STEP"<<std::endl;
			if (!(iss >> testLowerRange >> testUpperRange)) {
				//throw exception
				break;
			}
			currStep = FEATURES_RANGES_STEP;
		}
		else if (currStep == FEATURES_RANGES_STEP) {
			logger->sdebug()<<"FEATURES_RANGES_STEP"<<std::endl;
			FeatureRange featureRange;
			if (!(iss>>featureRange.featureIndex>>featureRange.a>>featureRange.b)) {
				break;
			}
			classRanges.push_back(featureRange);
		}
	}
	rangeFile.close();

	double **scaling = new double*[2];
	scaling[0] = new double[featureSize];
	scaling[1] = new double[featureSize];
	for (int i = 0; i < featureSize; i++) {
		scaling[0][i] = 0;
		scaling[1][i] = 0;
	}
	for (unsigned int i = 0; i < classRanges.size(); i++) {
		int featureIndex = classRanges[i].featureIndex - 1;
		if (featureIndex >= 0) {
			scaling[0][featureIndex] = classRanges[i].a;
			scaling[1][featureIndex] = classRanges[i].b;
		}
	}

	logger->debug("loadRangeFile exit");

	return scaling;
}
