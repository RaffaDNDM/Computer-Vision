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
	for (auto img : _input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)])
	{
		CannyDetector cd(img, _dataset_type);
		cd.detect();

		//cv::Mat dist(img.size(), CV_8U);
		//cv::distanceTransform(cd.getResult(), dist, cv::DIST_L2, 3);
		//cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
		//cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY);
		_canny_views.emplace_back(cd.getResult());
	}

	/*
	for (auto &img : _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)])
	{
		cv::Mat dist(img.size(), CV_8U);
		cv::distanceTransform(img, dist, cv::DIST_L2, 3);
		cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
		cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY);
		_filter_masks.emplace_back(img);
		cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], dist);
		cv::waitKey(0);
	}
	*/

	int img_num=0;
	for (auto img : _input_imgs[static_cast<int>(Dataset::PatternIMG::TEST)])
	{
		/*
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], img);
		cv::waitKey(0);
		*/

		//double abs_min = std::numeric_limits<double>::max();
		double abs_min = 0.0;
		int min_index = 0;
		cv::Point min_pos;

		int index = 0;
		/*
		for (auto filter : _canny_views)
		{
			for (int i = 0; i < (img.rows-filter.rows +1); i++)
			{
				for (int j = 0; j < (img.cols-filter.cols+1); j++)
				{
					cv::Mat filtered_img;
					cv::bitwise_and(img(cv::Range(i, i + filter.rows), cv::Range(j, j + filter.cols)), filter, filtered_img);

					CannyDetector cd(filtered_img, _dataset_type);
					cd.detect();

					cv::Mat dist(filtered_img.size(), CV_8U);
					cv::distanceTransform(cd.getResult(), dist, cv::DIST_L2, 3);
					cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
					cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY_INV);
					cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
					cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], dist);
					cv::waitKey(0);
					//std::cout << dist.size();
					cv::Mat result;
					cv::filter2D(dist, result, CV_8U, _canny_views[index], cv::Point(0, 0));

					Point min_point, max_point;
					cv::minMaxLoc(result, )
				
					//std::cout << result.size();
					
					if (result.at<int>(0, 0) < min)
					{
						min_i = i;
						min_j = j;
						min = result.at<int>(0, 0);
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
		*/

		for (auto filter : _canny_views)
		{
			CannyDetector cd(img, _dataset_type);
			cd.detect();
			
			cv::Mat result(cv::Size(img.rows-filter.rows+1, img.cols - filter.cols + 1), CV_32F);
			double min, max;
			cv::Point min_point, max_point;
			cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCORR);
			cv::minMaxLoc(result, &min, &max, &min_point, &max_point);

			if (max > abs_min)
			{
				min_pos = max_point;
				min_index = index;
				abs_min = max;
			}

			index++;
		}

		//if ((min_pos.y + _canny_views[min_index].rows) < img.rows && (min_pos.x + _canny_views[min_index].cols) < img.cols)
		//	std::cout << "OKKKKKKKKKKKKKKKKKKK" << std::endl;
		
		cv::Mat mask_result=img.clone();

		/*
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
		*/

		//cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], mask_result);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		//cv::waitKey(0);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_views[min_index]);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		//cv::waitKey(0);

		/*
		cv::Mat img_canny;
		_canny_imgs[min_index].convertTo(img_canny, CV_8UC1, 255.0 / (static_cast<double>(std::numeric_limits<int>::max())));
		
		switch (_canny_views[min_index].type()) {
			case CV_8U:  r = "8U"; break;
			case CV_8S:  r = "8S"; break;
			case CV_16U: r = "16U"; break;
			case CV_16S: r = "16S"; break;
			case CV_32S: r = "32S"; break;
			case CV_32F: r = "32F"; break;
			case CV_64F: r = "64F"; break;
			default:     r = "User"; break;
		}
		*/

		for (int i = 0; i < _canny_views[min_index].rows; i++)
		{
			for (int j = 0; j < _canny_views[min_index].cols; j++)
			{
				if (_canny_views[min_index].at<uchar>(i, j) > 100)
					mask_result.at<cv::Vec3b>(min_pos.y + i, min_pos.x + j) = cv::Vec3b(0, 0, 255);
			}
		}

		//cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], mask_result);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		//cv::waitKey(0);

		printBestMatch(img_num, min_index, abs_min, min_pos, mask_result);
		img_num++;
	}
}

void TemplateMatching::printBestMatch(int img_num, int min_index, double abs_min, 
	                                  cv::Point min_pos, cv::Mat mask_result)
{
	cv::imwrite(Dataset::output_path + Dataset::types[static_cast<int>(_dataset_type)]+
		        "/result"+std::to_string(img_num)+".jpg", mask_result);

	mutex.lock();
	std::cout << Dataset::colors[static_cast<int>(_dataset_type)] << "image " << img_num << ":" DEFAULT << std::setw(8)
		      << std::right << " mask"+ std::to_string(min_index) << Dataset::colors[static_cast<int>(_dataset_type)] 
		      << " >>> " << DEFAULT << abs_min << Dataset::colors[static_cast<int>(_dataset_type)] << " in " << DEFAULT 
		      << "(" << min_pos.y << "," << min_pos.x << ")" << std::endl;
	mutex.unlock();
}