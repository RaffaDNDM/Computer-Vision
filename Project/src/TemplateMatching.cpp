/**
	@file PanoramicImage.cpp
	@brief Implementation of the class used for panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
*/
#include "TemplateMatching.hpp"
#include "CannyDetector.hpp"

static std::mutex mutex;

TemplateMatching::TemplateMatching(cv::String path, Dataset::Type dataset_type) :
	_dataset_type{ dataset_type }
{
	if (path.back() == '\\' || path.back() == '/')
		loadImages(path + Dataset::types[static_cast<int>(dataset_type)]);
	else
		loadImages(path + "/" + Dataset::types[static_cast<int>(dataset_type)]);
}

void TemplateMatching::loadImages(cv::String path) 
{
	int i = 0;
	for (; i<static_cast<int>(Dataset::PatternIMG::SET_IMAGES); i++)
	{
		std::vector<cv::String> imgs_names;
		cv::utils::fs::glob(path+ Dataset::sub_folders[i], Dataset::patterns[i], imgs_names);

		if (imgs_names.size() == 0)
			break;
		else
		{
			std::vector<cv::Mat> tmp_imgs;
			for (cv::String name : imgs_names)
			{
				cv::Mat img = imread(name, cv::IMREAD_COLOR);
				tmp_imgs.emplace_back(img);
			}

			_input_imgs.emplace_back(tmp_imgs);
		}
	}

	if (i < static_cast<int>(Dataset::PatternIMG::SET_IMAGES))
		throw InputIMGException();

	mutex.lock();
	stateAcquisitionImages(path);
	mutex.unlock();
}

void TemplateMatching::stateAcquisitionImages(cv::String path)
{
	std::cout << std::endl << Dataset::bold_colors[static_cast<int>(_dataset_type)] << path << std::endl;
	std::cout << LINE;
	std::cout << "Dataset of " << Dataset::types[static_cast<int>(_dataset_type)] << DEFAULT << std::endl;
	std::cout << "Number of masks images:   " << Dataset::colors[static_cast<int>(_dataset_type)]
		      << _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)].size() << DEFAULT << std::endl;
	std::cout << "Number of views images:   " << Dataset::colors[static_cast<int>(_dataset_type)]
			  << _input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)].size() << DEFAULT << std::endl;
	std::cout << "Number of test images:    " << Dataset::colors[static_cast<int>(_dataset_type)] 
		      << _input_imgs[static_cast<int>(Dataset::PatternIMG::TEST)].size() << std::endl;
	std::cout << Dataset::bold_colors[static_cast<int>(_dataset_type)] << LINE << DEFAULT << std::endl;
}

void TemplateMatching::cannyDetection()
{
	for (cv::Mat img : _input_imgs[static_cast<int>(Dataset::PatternIMG::TEST)])
	{
		/*
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], img);
		cv::waitKey(0);
		*/
		CannyDetector cd(img, _dataset_type);
		cd.detect();
		_canny_imgs.emplace_back(cd.getResult().clone());

		cv::Mat dist(img.size(), CV_8UC3);
		cv::distanceTransform(_canny_imgs.back(), dist, cv::DIST_L2, 3);
		cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
		cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], dist);
		cv::waitKey(0);
	}
}