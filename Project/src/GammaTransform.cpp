#include "GammaTransform.hpp"

GammaTransform::GammaTransform(float gamma)
{
	for (int i = 0; i < 256; i++)
		_values.emplace_back(cv::saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f));
}

void GammaTransform::computeTransform(cv::Mat &src, cv::Mat &dst)
{
	dst = src.clone();
	const int channels = dst.channels();

	switch (channels)
	{
		case 1:
		{
			cv::MatIterator_<uchar> it, end;

			for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
				*it = _values[(*it)];

			break;
		}

		case 3:
		{
			cv::MatIterator_<cv::Vec3b> it, end;

			for (it = dst.begin<cv::Vec3b>(), end = dst.end<cv::Vec3b>(); it != end; it++)
			{
				(*it)[0] = _values[((*it)[0])];
				(*it)[1] = _values[((*it)[1])];
				(*it)[2] = _values[((*it)[2])];
			}

			break;
		}
	}
}