#include "Project.hpp"

#ifndef GAMMA
#define GAMMA
class GammaTransform 
{
public:
	GammaTransform(float gamma);

	void computeTransform(cv::Mat& src, cv::Mat& dst);

private:
	std::vector<uchar> _values;
};

#endif