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
				cv::Mat img;
				if (i == static_cast<int>(Dataset::PatternIMG::MASKS))
					img = imread(name, cv::IMREAD_GRAYSCALE);
				else
					img = imread(name, cv::IMREAD_COLOR);
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
	Dataset::Parameter param = Dataset::canny_params[static_cast<int>(_dataset_type)];

	for (auto img : _input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)])
	{
		switch (static_cast<int>(_dataset_type))
		{
			case 0:
				gammaTransform(img, img, 0.8);
				break;

			case 1:
				gammaTransform(img, img, 1.2);
				break;

			case 2:
				break;
		}
		CannyDetector cd(img, _dataset_type, param.threshold_template, param.threshold_template);
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

	int img_index = 0;

	for (auto img : _input_imgs[static_cast<int>(Dataset::PatternIMG::TEST)])
	{
		/*
		cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], img);
		cv::waitKey(0);
		*/

		//double abs_min = std::numeric_limits<double>::max();
		double abs_max = 0.0;
		int max_index = 0;
		cv::Point max_pos;

		int mask_index = 0;
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
		
		BestResults r;
		equalization(img);
		//gammaTransform(img, img, 0.5);
		CannyDetector cd(img, _dataset_type, param.threshold_test, param.threshold_test);
		cd.detect();

		for (auto filter : _canny_views)
		{	
			cv::Mat result(cv::Size(img.rows-filter.rows+1, img.cols - filter.cols + 1), CV_32F);
			double min_score, max_score;
			cv::Point min_point, max_point;
			//cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCOEFF_NORMED); //best for duck
			cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCOEFF); //best for 
			//cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCORR_NORMED);
			//cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCORR);
			cv::minMaxLoc(result, &min_score, &max_score, &min_point, &max_point);
			r.insert(max_point, img_index, mask_index, max_score);
			
			//findMax(result, r, img_index, mask_index);

			mask_index++;
		}

		//if ((min_pos.y + _canny_views[min_index].rows) < img.rows && (min_pos.x + _canny_views[min_index].cols) < img.cols)
		//	std::cout << "OKKKKKKKKKKKKKKKKKKK" << std::endl;

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

		cv::Mat mask_result=img.clone();

		for(auto res: r.getBestResults())
		{
			for (int i = 0; i < _canny_views[res.getMaskIndex()].rows; i++)
			{
				for (int j = 0; j < _canny_views[res.getMaskIndex()].cols; j++)
				{
					if (_canny_views[res.getMaskIndex()].at<uchar>(i, j) > 100)
						mask_result.at<cv::Vec3b>(res.getPoint().y + i, res.getPoint().x + j) = cv::Vec3b(0, 0, 255);
				}
			}
		}
		
		printBestMatch(r, mask_result);

		//cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], mask_result);
		//cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], _canny_masks.back());
		//cv::waitKey(0);
		

		img_index++;
	}
}

void TemplateMatching::printBestMatch(BestResults best_results, cv::Mat mask_result)
{
	std::vector<Result> results = best_results.getBestResults();
	cv::imwrite(Dataset::output_path + Dataset::types[static_cast<int>(_dataset_type)] +
		"/result" + std::to_string(results[0].getImageIndex()) + ".jpg", mask_result);

	mutex.lock();
	
	for(auto r : results)
	{
		std::cout << Dataset::colors[static_cast<int>(_dataset_type)] << "image " << r.getImageIndex() << ":" DEFAULT << std::setw(8)
			<< std::right << " mask" + std::to_string(r.getMaskIndex()) << Dataset::colors[static_cast<int>(_dataset_type)]
			<< " >>> " << DEFAULT << std::setw(11) << std::left << r.getScore() << Dataset::colors[static_cast<int>(_dataset_type)]
			<< " in " << DEFAULT << "(" << r.getPoint().y << "," << r.getPoint().x << ")" << std::endl;
	}

	mutex.unlock();
}

void TemplateMatching::findMax(cv::Mat result, BestResults& r, int img_index, int mask_index)
{
	cv::String x;
	switch (result.type()) {
	case CV_8U:  x = "8U"; break;
	case CV_8S:  x = "8S"; break;
	case CV_16U: x = "16U"; break;
	case CV_16S: x = "16S"; break;
	case CV_32S: x = "32S"; break;
	case CV_32F: x = "32F"; break;
	case CV_64F: x = "64F"; break;
	default:     x = "User"; break;
	}

	for (int i = 0; i < result.rows; i++)
		for (int j = 0; j < result.cols; j++)
			r.insert(cv::Point(j,i), img_index, mask_index, result.at<float>(i,j));
}

void TemplateMatching::gammaTransform(cv::Mat &src, cv::Mat &dst, float gamma)
{
	unsigned char lut[256];

	for (int i = 0; i < 256; i++)
		lut[i] = cv::saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);

	dst = src.clone();
	const int channels = dst.channels();

	switch (channels)
	{
		case 1:
		{
			cv::MatIterator_<uchar> it, end;

			for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
				*it = lut[(*it)];

			break;
		}

		case 3:
		{
			cv::MatIterator_<cv::Vec3b> it, end;

			for (it = dst.begin<cv::Vec3b>(), end = dst.end<cv::Vec3b>(); it != end; it++)
			{
				(*it)[0] = lut[((*it)[0])];
				(*it)[1] = lut[((*it)[1])];
				(*it)[2] = lut[((*it)[2])];
			}

			break;
		}
	}
}

void TemplateMatching::equalization(cv::Mat& img)
{
	std::vector<cv::Mat> img_planes;
	cv::split(img, img_planes);

	for (int i = 0; i < img.channels(); i++)
		cv::equalizeHist(img_planes[i], img_planes[i]);

	cv::merge(img_planes, img);
}