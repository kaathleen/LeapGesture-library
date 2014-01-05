#ifndef KFOLDCROSSVALIDATION_H_
#define KFOLDCROSSVALIDATION_H_

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdexcept>

template <class T>
class KFoldCrossValidation
{
public:
	KFoldCrossValidation(std::vector<T> dataset, int k);

	T getLearningSetElement(int elementIndex, int foldNumber);

	int getLearningSetSize(int foldNumber);

	T getTestingSetElement(int elementIndex, int foldNumber);

	int getTestingSetSize(int foldNumber);

	int getK()
	{
		return this->k;
	}

private:
	int k;
	std::vector<T> dataset;
	std::vector<int> subsamplesSizesGrowths;

	void prepareDataset();
	void computeGrowthsOfSubsamplesSizes();
	void swapDatasetElements(int index1, int index2);
	int getLearningSetElementIndex(int elementIndex, int foldNumber);
	int getTestingSetElementIndex(int elementIndex, int foldNumber);
	int getSubsampleSize(int subsampleIndex);

	void checkFoldNumber(int foldNumber);
	void checkLearningSetElementIndex(int elementIndex, int foldNumber);
	void checkTestingSetElementIndex(int elementIndex, int foldNumber);
};

template <class T>
KFoldCrossValidation<T>::KFoldCrossValidation(std::vector<T> dataset, int k)
{
	this->k = k;
	this->dataset = dataset;

	if (k < 2)
	{
		throw std::invalid_argument("K must be greater than 1");
	}
	if (dataset.size() < k)
	{
		throw std::invalid_argument("Dataset size cannot be less than k value");
	}

	prepareDataset();
}

template <class T>
T KFoldCrossValidation<T>::getLearningSetElement(int elementIndex, int foldNumber)
{
	checkFoldNumber(foldNumber);
	checkLearningSetElementIndex(elementIndex, foldNumber);

	int redesLearningSetElementIndex =
			getLearningSetElementIndex(elementIndex, foldNumber);

	return this->dataset[redesLearningSetElementIndex];
}

template <class T>
int KFoldCrossValidation<T>::getLearningSetSize(int foldNumber)
{
	checkFoldNumber(foldNumber);

	return dataset.size() - getSubsampleSize(foldNumber);
}

template <class T>
T KFoldCrossValidation<T>::getTestingSetElement(int elementIndex, int foldNumber)
{
	checkFoldNumber(foldNumber);
	checkTestingSetElementIndex(elementIndex, foldNumber);

	int redesTestingSetElementIndex =
			getTestingSetElementIndex(elementIndex, foldNumber);

	return this->dataset[redesTestingSetElementIndex];
}

template <class T>
int KFoldCrossValidation<T>::getTestingSetSize(int foldNumber)
{
	checkFoldNumber(foldNumber);

	return getSubsampleSize(foldNumber);
}

template <class T>
void KFoldCrossValidation<T>::prepareDataset()
{
	// mixing data
	srand (time(NULL));
	for (int i=0; i<this->dataset.size(); i++)
	{
		int index1 = rand() % this->dataset.size();
		int index2 = rand() % this->dataset.size();
		swapDatasetElements(index1, index2);
	}

	// computing growths of folds sizes
	computeGrowthsOfSubsamplesSizes();
}

template <class T>
void KFoldCrossValidation<T>::computeGrowthsOfSubsamplesSizes()
{
	int subsamplesSize = this->dataset.size() / this->k;
	int deficiencyValue = this->dataset.size() % this->k;
	int lastSubsamplesSize = 0;
	for (int i=0; i<this->k; i++)
	{
		int currSubsamplesSize = subsamplesSize + lastSubsamplesSize;
		if (deficiencyValue>0)
		{
			currSubsamplesSize++;
			deficiencyValue--;
		}
		this->subsamplesSizesGrowths.push_back(currSubsamplesSize);

		lastSubsamplesSize = currSubsamplesSize;
	}
}

template <class T>
void KFoldCrossValidation<T>::swapDatasetElements(int index1, int index2)
{
	T tempEl = this->dataset[index1];
	this->dataset[index1] = this->dataset[index2];
	this->dataset[index2] = tempEl;
}

template <class T>
int KFoldCrossValidation<T>::getLearningSetElementIndex(int elementIndex, int foldNumber)
{
	int result = elementIndex;

	int testingSubsampleStartIndex = 0;
	if (foldNumber > 0)
		testingSubsampleStartIndex = subsamplesSizesGrowths[foldNumber-1];

	if (elementIndex >= testingSubsampleStartIndex)
	{
		result += getSubsampleSize(foldNumber);
	}

	return result;
}

template <class T>
int KFoldCrossValidation<T>::getTestingSetElementIndex(int elementIndex, int foldNumber)
{
	int result = elementIndex;

	int testingSubsampleStartIndex = 0;
	if (foldNumber > 0)
		testingSubsampleStartIndex = subsamplesSizesGrowths[foldNumber-1];

	return result + testingSubsampleStartIndex;
}

template <class T>
int KFoldCrossValidation<T>::getSubsampleSize(int subsampleIndex)
{
	if (subsampleIndex < 0)
		return 0;

	int subsampleSize = subsamplesSizesGrowths[subsampleIndex];
	if (subsampleIndex>0)
	{
		subsampleSize -= subsamplesSizesGrowths[subsampleIndex-1];
	}

	return subsampleSize;
}

template <class T>
void KFoldCrossValidation<T>::checkFoldNumber(int foldNumber)
{
	if (foldNumber >= k || foldNumber < 0)
	{
		throw std::invalid_argument("Fold number must be less than k value and greater or equal zero");
	}
}

template <class T>
void KFoldCrossValidation<T>::checkLearningSetElementIndex(int elementIndex, int foldNumber)
{
	if (elementIndex >= getLearningSetSize(foldNumber) || elementIndex < 0)
	{
		throw std::invalid_argument("Element index must be less than learning set size and greater or equal than zero");
	}
}

template <class T>
void KFoldCrossValidation<T>::checkTestingSetElementIndex(int elementIndex, int foldNumber)
{
	if (elementIndex >= getTestingSetSize(foldNumber) || elementIndex < 0)
	{
		throw std::invalid_argument("Element index must be less than testing set size and greater or equal than zero");
	}
}

#endif /* KFOLDCROSSVALIDATION_H_ */
