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
	for (cv::Mat img : _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)])
	{
		CannyDetector cd(img, _dataset_type);
		cd.detect();

		cv::Mat dist(img.size(), CV_8U);
		cv::distanceTransform(cd.getResult(), dist, cv::DIST_L2, 3);
		cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
		cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY);
		_canny_masks.emplace_back(dist);
	}

	int img_num=0;
	for (cv::Mat img : _input_imgs[static_cast<int>(Dataset::PatternIMG::TEST)])
	{
		/*
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], img);
		cv::waitKey(0);
		*/
		CannyDetector cd(img, _dataset_type);
		cd.detect();

		cv::Mat dist(img.size(), CV_8U);
		cv::distanceTransform(cd.getResult(), dist, cv::DIST_L2, 3);
		cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
		cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY_INV);
		cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], dist);
		cv::waitKey(0);

		uchar min = 255;
		int min_index = 0;
		int min_i = 0;
		int min_j = 0;
		int index = 0;
		for (auto filter : _canny_masks)
		{
			//std::cout << dist.size();
			cv::Mat result;
			cv::filter2D(dist, result, CV_8U, filter);
		
			/*
			Point min_point, max_point;
			cv::minMaxLoc(result, )
			*/

			//std::cout << result.size();
			for (int i = 0; i < (result.rows-filter.rows); i++)
			{
				for (int j = 0; j < (result.cols-filter.cols); j++)
				{
					if (result.at<uchar>(i, j) < min)
					{
						min_i = i;
						min_j = j;
						min = result.at<uchar>(i, j);
						min_index = index;
					}
				}
			}
			//std::cout << result.at<uchar>(i, j) << std::endl;

			//cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
			//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], result);
			//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
			//cv::waitKey(0);
			index++;
		}

		if ((min_i + _canny_masks[min_index].rows) < img.rows && (min_j + _canny_masks[min_index].cols) < img.cols)
			std::cout << "OKKKKKKKKKKKKKKKKKKK" << std::endl;
		
		cv::Mat mask_result=img.clone();

		cv::String r;
		switch (mask_result.type()) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default:     r = "User"; break;
		}

		std::cout << r << std::endl;

		cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], mask_result);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		//cv::waitKey(0);
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks[min_index]);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		cv::waitKey(0);

		cv::Mat img_canny;
		//_canny_imgs[min_index].convertTo(img_canny, CV_8UC1, 255.0 / (static_cast<double>(std::numeric_limits<int>::max())));
		
		switch (_canny_masks[min_index].type()) {
			case CV_8U:  r = "8U"; break;
			case CV_8S:  r = "8S"; break;
			case CV_16U: r = "16U"; break;
			case CV_16S: r = "16S"; break;
			case CV_32S: r = "32S"; break;
			case CV_32F: r = "32F"; break;
			case CV_64F: r = "64F"; break;
			default:     r = "User"; break;
		}

		for (int i = 0; i < _canny_masks[min_index].rows; i++)
		{
			for (int j = 0; j < _canny_masks[min_index].cols; j++)
			{
				if (_canny_masks[min_index].at<int>(i, j) > 100)
					mask_result.at<cv::Vec3b>(min_i + i, min_j + j) = cv::Vec3b(0, 0, 255);
			}
		}

		cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], mask_result);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		cv::waitKey(0);
		
		std::cout << "Image "<< img_num << ":   mask" << min_index << ">>" << (int) min 
			      << "<-------(" << min_i <<","<<min_j<<")"<<std::endl;

		img_num++;
	}
}