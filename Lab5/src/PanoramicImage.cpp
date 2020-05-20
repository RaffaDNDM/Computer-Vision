/**
	@file PanoramicImage.cpp
	@brief Implementation of the class used for panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "PanoramicImage.h"

PanoramicImage::PanoramicImage(cv::String path, float ratio):
	_ratio{ratio}
{
	loadImages(path);
	cylinderProjection();
}

PanoramicImage::PanoramicImage(std::vector<cv::Mat> &projected_imgs, float ratio):
	_ratio{ ratio }
{
	_projected_imgs.reserve(projected_imgs.size());
	copy(projected_imgs.begin(), projected_imgs.end(), std::back_inserter(_projected_imgs));
}

void PanoramicImage::panoramicImage(bool isORB)
{
	cv::Ptr<cv::BFMatcher> matcher;

	if (isORB)
	{
		method_name = "ORB";
		cv::Ptr<cv::ORB> orb;
		key_desc<cv::ORB>(orb);
		matcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
	}
	else
	{
		method_name = "SIFT";
		cv::Ptr<cv::xfeatures2d::SIFT> sift;
		key_desc<cv::xfeatures2d::SIFT>(sift);
		matcher = cv::BFMatcher::create(cv::NORM_L2, true);
	}
	
	//Compute thresholds and matches
	std::vector<std::vector<cv::DMatch>> matches;
	match(matcher, matches, _thresholds);

	//Compute translations
	translate_imgs(_translations, matches, _thresholds);
	
	final_image(_translations);
}

void PanoramicImage::loadImages(cv::String path)
{
	for(cv::String pattern : patterns)
	{
		std::vector<cv::String> imgs_names;
		cv::utils::fs::glob(path, pattern, imgs_names);
		
		if (imgs_names.size() == 0)
			continue;
		else
		{
			for (cv::String name : imgs_names)
			{
				cv::Mat img = imread(name, cv::IMREAD_COLOR);
				_input_imgs.push_back(img);
			}
		}
	}

	std::cout << LINE << "Number of input images:   " << _input_imgs.size() << "\n" << LINE;
}

void PanoramicImage::cylinderProjection()
{
	for (cv::Mat& img : _input_imgs)
	{
		cv::Mat projected_img = PanoramicUtils::cylindricalProj(img, FOV / 2);
		_projected_imgs.push_back(projected_img);
	}

	for (cv::Mat& img_r : _projected_imgs)
	{
		cv::equalizeHist(img_r, img_r);
	}
}

float PanoramicImage::findMin(std::vector<cv::DMatch> d_matches)
{
	float min = std::numeric_limits<float>::max();

	for (cv::DMatch match : d_matches)
	{
		if (match.distance < min)
			min = match.distance;
	}

	return min;
}

template <class T>
void PanoramicImage::key_desc(cv::Ptr<T>& method)
{
	method = T::create();

	for (cv::Mat &img : _projected_imgs)
	{
		std::vector<cv::KeyPoint> kpts;
		cv::Mat desc;

		method->detectAndCompute(img, cv::Mat(), kpts, desc);

		descriptors.push_back(desc);
		keypoints.push_back(kpts);
	}
}

void PanoramicImage::match(cv::Ptr<cv::BFMatcher> matcher, 
						   std::vector<std::vector<cv::DMatch>>& matches, 
	                       std::vector<float>& thresholds)
{
	for (int i = 1; i < descriptors.size(); i++)
	{
		std::vector<cv::DMatch> match;
		matcher->match(descriptors[i - 1], descriptors[i], match);
		float min = findMin(match);
		matches.push_back(match);
		thresholds.push_back(_ratio * min);
	}


	/**
		cv::Mat res;
		cv::drawMatches(img1, kpts1, img2, kpts2, matches, res, Scalar::all(-1), Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	*/
	std::cout << LINE;
}

void PanoramicImage::translate_imgs(std::vector<cv::Point2f>& translations,
									std::vector<std::vector<cv::DMatch>> matches,
                                    std::vector<float> thresholds)
{

	for (int i = 0; i < matches.size(); i++)
	{
		std::vector<cv::Point2f> pointsO;
		std::vector<cv::Point2f> pointsS;
		cv::Mat mask;

		for (int j = 0; j < matches[i].size(); j++)
		{
			if (matches[i][j].distance <= thresholds[i])
			{
				//indice del keypoint dell'immagine i-1 
				pointsO.push_back(keypoints[i][matches[i][j].queryIdx].pt);

				//indice del keypoint dell'immagine i
				pointsS.push_back(keypoints[i + 1][matches[i][j].trainIdx].pt);

			}
		}

		cv::Mat H = cv::findHomography(pointsO, pointsS, mask, cv::RANSAC);

		cv::Point2f transl(0.0, 0.0);

		int k = 0;
		int count = 0;
		for (; k < mask.rows; k++)
		{
			if ((unsigned int)mask.at<uchar>(k))
			{
				count++;
				transl.x += (pointsO[k].x - pointsS[k].x);
				transl.y += (pointsO[k].y - pointsS[k].y);
			}
		}

		transl.x /= count;
		transl.y /= count;
		translations.push_back(transl);
	}
}

void PanoramicImage::final_image(std::vector<cv::Point2f>& translations)
{
	std::vector<cv::Point2f> final_translations;

	int min_height=0;
	int max_height=_projected_imgs[0].rows;
	int current_height=0;
	int width = _projected_imgs[0].cols;
	for (int i = 1; i < _projected_imgs.size(); i++)
	{
		//translations[i - 1].x = (translations[i - 1].x < 0) ? _projected_imgs[i - 1].cols : translations[i - 1].x;
		float x = (translations[i - 1].x < 0) ? _projected_imgs[i - 1].cols : translations[i - 1].x;
		final_translations.push_back(cv::Point2f(cvRound(x), cvRound(translations[i-1].y)));

		current_height += final_translations[i - 1].y;

		if (current_height < min_height)
		{
			min_height += (current_height - min_height);
		}
		else if((current_height+_projected_imgs[i].rows) > max_height)
		{
			max_height += (_projected_imgs[i].rows + current_height - max_height);
		}

		width += final_translations[i-1].x;
	}

	_panoramic_view = cv::Mat(cv::Size(width, max_height-min_height), _projected_imgs[0].type());
	int x1 = 0;
	int x2 = _projected_imgs[0].cols;
	int y1 = 0-min_height;
	int y2 = y1 + _projected_imgs[0].rows;
	cv::Mat img = _panoramic_view(cv::Range(y1, y2), cv::Range(x1, x2));
	_projected_imgs[0].copyTo(img);

	for (int i = 1; i < _projected_imgs.size(); i++)
	{
		x1 += cvRound(final_translations[i - 1].x);
		x2 = x1 + _projected_imgs[i].cols;
		y1 += cvRound(final_translations[i - 1].y);
		y2 += cvRound(final_translations[i - 1].y);

		img = _panoramic_view(cv::Range(y1, y2), cv::Range(x1, x2));
		_projected_imgs[i].copyTo(img);
	}

	cv::equalizeHist(_panoramic_view, _panoramic_view);
}

std::vector<cv::Mat> PanoramicImage::getProjected()
{
	return _projected_imgs;
}

cv::Mat PanoramicImage::getResult()
{
	return _panoramic_view;
}

void PanoramicImage::printInfo()
{
	std::cout << LINE << "[" << method_name << "] Thresholds\n" << LINE;

	for (int i = 0; i < _thresholds.size(); i++)
		std::cout << i << ":  " << _thresholds[i] << std::endl;

	std::cout << LINE << "[" << method_name << "] Translations\n" << LINE;

	for (int i = 0; i < _translations.size(); i++)
		std::cout << "i: " << i + 1 << "     " << _translations[i] << std::endl;

	std::cout << LINE;
}