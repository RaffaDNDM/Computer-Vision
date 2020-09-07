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
	/**
		@brief Constructor of gamma transform.
		@param gamma gamma coefficent
	*/
	GammaTransform(float gamma);

	/**
		@brief Compute the gamma transform of an image.
		@param src input image
		@param dst transformed input image
	*/
	void computeTransform(cv::Mat& src, cv::Mat& dst);

private:
	//Association between level and transformed level
	std::vector<uchar> _values; 
};

#endif