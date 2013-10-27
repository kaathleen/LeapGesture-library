#include <iostream>
#include <Eigen/Eigen>
#include <vector>
#include <cmath>

template <typename T> int sgn(T val)
{
    return (val > T(0)) - (val < T(0));
}

class LeapMotionsCalibration
{
	Eigen::MatrixXf P, Q;
public:
	LeapMotionsCalibration();
	
	void readData(std::vector<Eigen::Vector3f> firstSet, std::vector<Eigen::Vector3f> secondSet);
	
	Eigen::Matrix4f calculateTransformation();
	
	double calculateError(std::vector<Eigen::Vector3f> firstSet, 
		std::vector<Eigen::Vector3f> secondSet, Eigen::Matrix4f transformation);
};
