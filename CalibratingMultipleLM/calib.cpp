#include "calib.h"

LeapMotionsCalibration::LeapMotionsCalibration()
{
	
}


void LeapMotionsCalibration::readData(std::vector<Eigen::Vector3f> firstSet, 
		std::vector<Eigen::Vector3f> secondSet)
{
	P.resize(firstSet.size(), 3);
	Q.resize(secondSet.size(), 3);
	
	int i = 0;
	for (std::vector<Eigen::Vector3f>::iterator it = firstSet.begin() ; it != firstSet.end(); ++it, i++)
	{
		P.block<1,3>(i,0) = (*it).head<3>();
	}
	
	i = 0;
	for (std::vector<Eigen::Vector3f>::iterator it = secondSet.begin() ; it != secondSet.end(); ++it, i++)
	{
		Q.block<1,3>(i,0) = (*it).head<3>();
	}
}
	
Eigen::Matrix4f LeapMotionsCalibration::calculateTransformation()
{
	float mean[6];
	memset(mean, 0, 6 * sizeof(float));
	
	// Calculating the mean of (x,y,z) of points in P & Q matrices		
	for(int j=0;j<P.rows();j++)
	{
		for(int h=0;h<3;h++)
		{
			mean[h] += P(j,h);
			mean[h+3] += Q(j,h);
		}
	}
	for(int j=0;j<6;j++) mean[j] /= P.rows();
		
	// Subtracting the mean
	for(int j=0;j<P.rows();j++)
	{
		for(int h=0;h<3;h++) 
		{
			P(j,h) -= mean[h];
			Q(j,h) -= mean[h+3];
		}
	} 
			
	// calculating  P * Q to find optimal Rotation
	Eigen::MatrixXf A = P.transpose() * Q;
			
	// SVD	
	Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
	Eigen::MatrixXf V = svd.matrixU(), W = svd.matrixV();

	// To make it right-hand
	Eigen::Matrix3f U = Eigen::MatrixXf::Identity(3,3);
	U(2,2) = sgn(A.determinant());
			
	// Rotation
	U = W * U * V.transpose();
			
	// Translation
	Eigen::Vector3f T;		
	T[0] = - mean[0];
	T[1] = - mean[1];
	T[2] = - mean[2]; 
	T = U*T;
	T[0] += mean[3];
	T[1] += mean[4];
	T[2] += mean[5]; 
	
	Eigen::Matrix4f optimal;
	optimal <<  Eigen::Matrix4f::Identity();
	optimal.block<3,3>(0,0) = U.block<3,3>(0,0);
	optimal.block<3,1>(0,3) = T.head<3>();

	return optimal;
}
	
double LeapMotionsCalibration::calculateError(std::vector<Eigen::Vector3f> firstSet, 
		std::vector<Eigen::Vector3f> secondSet, Eigen::Matrix4f transformation)
{
	
	if ( firstSet.size() != secondSet.size() )
	{
		std::cout<< "Different size of sets" << std::endl;
		return 0.0;
	}
	
	double error = 0.0;
	for (std::vector<Eigen::Vector3f>::iterator it = firstSet.begin(), it2 = secondSet.begin() ; 
		it != firstSet.end(); ++it, ++it2)
	{
		Eigen::Vector4f tmp;
		tmp.head<3>() = (*it).head<3>();
		tmp[3] =  1.0;
		tmp = transformation * tmp;
		
		
		error += ( tmp.head<3>() - (*it2)).norm();
	}
	
	error = error / firstSet.size();
	return error;
}
