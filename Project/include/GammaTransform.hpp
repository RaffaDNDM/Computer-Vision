/**
	@file GammaTransform.hpp
	@brief Class for gamma transform.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef GAMMA
#define GAMMA

#include "Project.hpp"
#include "Utility.hpp"

class GammaTransform 
{
public:
	GammaTransform(float gamma);

	void computeTransform(cv::Mat& src, cv::Mat& dst);

private:
	std::vector<uchar> _values;
};

#endif