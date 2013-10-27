#include <iostream>
#include <stdio.h>
#include <Eigen/Eigen>
#include <vector>
#include "calib.h"
#include <fstream>

int main()
{
	LeapMotionsCalibration calib;
	std::vector<Eigen::Vector3f> firstSet, secondSet;

	std::ifstream o;
	o.open("calib.txt");
	int pairs;
	o>>pairs;
	for(int i=0;i<pairs;i++)
	{
		Eigen::Vector3f tmp;
		o >> tmp[0]; o >> tmp[1]; o >> tmp[2];
		std::cout<<"( " << tmp[0] <<" " <<tmp[1] << " "<<tmp[2]<<" )"<< std::endl;
		firstSet.push_back(tmp);
		
		o >> tmp[0]; o >> tmp[1]; o >> tmp[2];
		std::cout<<"[ " << tmp[0] <<" " <<tmp[1] << " "<<tmp[2]<<" ]"<< std::endl;
		secondSet.push_back(tmp);
	}
	o.close();
	
	
	calib.readData(firstSet, secondSet);
	Eigen::Matrix4f result = calib.calculateTransformation();
	double error = calib.calculateError(firstSet, secondSet, result); 
	
	std::cout<<"Calculated transformation between two LeapMotion coordinate systems is :"<<std::endl;
	std::cout<<result<<std::endl;
	std::cout<<"RMS error: " << error << std::endl;
	
	return 0;
}
