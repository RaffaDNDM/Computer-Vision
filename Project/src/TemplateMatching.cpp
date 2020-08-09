/**
	@file PanoramicImage.cpp
	@brief Implementation of the class used for panoramic view.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "TemplateMatching.hpp"

TemplateMatching::TemplateMatching(std::string path, Dataset::Type dataset_type) :
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
		std::cout << path << std::endl;
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

	std::cout << i << std::endl;
	if (i < static_cast<int>(Dataset::PatternIMG::SET_IMAGES))
		throw InputIMGException();
}

void TemplateMatching::printInfo()
{
	std::cout << LINE << "Dataset of " << Dataset::types[static_cast<int>(_dataset_type)] << std::endl;
	std::cout << "Number of masks images:   " << _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)].size() << std::endl;
	std::cout << "Number of views images:   " << _input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)].size() << std::endl;
	std::cout << "Number of test images:    " << _input_imgs[static_cast<int>(Dataset::PatternIMG::TEST)].size() << "\n" << LINE;
}

