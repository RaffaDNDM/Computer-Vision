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
	GammaTransform g(Dataset::gammas[static_cast<int>(_dataset_type)]);

	for (auto& img : _input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)])
	{
		cv::Mat tmp=img.clone();
		g.computeTransform(img, tmp);
		CannyDetector cd(img, _dataset_type, param.threshold_template, param.threshold_template-20.0);
		cd.detect();

		//cv::Mat dist(img.size(), CV_8U);
		//cv::distanceTransform(cd.getResult(), dist, cv::DIST_L2, 3);
		//cv::normalize(dist, dist, 0.0, 1.0, cv::NORM_MINMAX);
		//cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY);
		_canny_views.emplace_back(cd.getResult());
	}

	computeHist(_input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)]);

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
		
		cv::Mat img_copy = img.clone();
		BestResults r1(50), r2(10);
		//equalization(img_copy);
		CannyDetector cd(img_copy, _dataset_type, param.threshold_test, param.threshold_test-30.0);
		cd.detect();

		for (auto filter : _canny_views)
		{	
			cv::Mat result(cv::Size(img_copy.rows-filter.rows+1, img_copy.cols - filter.cols + 1), CV_32F);
			double min_score, max_score;
			cv::Point min_point, max_point;
			//cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCOEFF_NORMED); //best for duck
			cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCOEFF); //best for 
			//cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCORR_NORMED);
			//cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCORR);
			cv::normalize(result, result);
			cv::minMaxLoc(result, &min_score, &max_score, &min_point, &max_point);
			/*
			cv::Mat detected_patch = img(cv::Range(max_point.y, max_point.y + filter.rows),
				                         cv::Range(max_point.x, max_point.x + filter.cols)).clone();

			cv::Mat mask = _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)][mask_index];

			for(int i=0; i< mask.rows; i++)
				for (int j = 0; j < mask.cols; j++)
				{
					if (mask.at<uchar>(i, j) < 50)
					{
						detected_patch.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
					}
				}
			*/

			//double score = compareHistH(detected_patch, _input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)][mask_index]);
			//r.insert(max_point, img_index, mask_index, score);
			//std::cout << score;
			r1.insert(max_point, img_index, mask_index, max_score);
			
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

		for (auto res : r1.getBestResults())
		{
			cv::Mat detected_patch = img(cv::Range(res.getPoint().y,
				                                   res.getPoint().y + _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)][res.getMaskIndex()].rows),
				                         cv::Range(res.getPoint().x, 
											       res.getPoint().x + _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)][res.getMaskIndex()].cols))
				                         .clone();

			cv::Mat mask = _input_imgs[static_cast<int>(Dataset::PatternIMG::MASKS)][res.getMaskIndex()].clone();

			double count=0;
			double count_w = 0;
			for (int i = 0; i < mask.rows; i++)
				for (int j = 0; j < mask.cols; j++)
				{
					if (mask.at<uchar>(i, j) < 50)
					{
						detected_patch.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
					}
					else
					{
						count+=1.0;
						if (_input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)][res.getMaskIndex()].at<cv::Vec3b>(i, j)[0] > 200 &&
							_input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)][res.getMaskIndex()].at<cv::Vec3b>(i, j)[1] > 200 &&
							_input_imgs[static_cast<int>(Dataset::PatternIMG::VIEWS)][res.getMaskIndex()].at<cv::Vec3b>(i, j)[2] > 200)
							count_w+=1.0;
					}
				}			
			
			//g.computeTransform(detected_patch, detected_patch);

			if(count/count_w > 0.9)
				r2.insert(res.getPoint(), res.getImageIndex(), res.getMaskIndex(), res.getScore());
			else
			{
				double score = compareHistH(detected_patch, res.getMaskIndex());
				//std::cout << score << std::endl;
				r2.insert(res.getPoint(), res.getImageIndex(), res.getMaskIndex(), res.getScore() + score);
			}

			/*
			cv::namedWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
			cv::imshow("Canny" + Dataset::types[static_cast<int>(_dataset_type)], detected_patch);
			cv::waitKey(0);
			cv::destroyWindow("Canny" + Dataset::types[static_cast<int>(_dataset_type)]);
			*/
		}

		cv::Mat mask_result = img.clone();

		for(auto res: r2.getBestResults())
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
		
		printBestMatch(r2, mask_result);

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

void TemplateMatching::equalization(cv::Mat& img)
{
	std::vector<cv::Mat> img_planes;
	cv::split(img, img_planes);

	for (int i = 0; i < img.channels(); i++)
		cv::equalizeHist(img_planes[i], img_planes[i]);

	cv::merge(img_planes, img);
}

void TemplateMatching::computeHist(std::vector<cv::Mat> imgs)
{
	for(auto& img : imgs)
	{
		cv::Mat tmp;
		cvtColor(img, tmp, cv::COLOR_BGR2HSV);

		int h_bins = 45;
		float h_ranges[] = { 0, 180 };
		const float* ranges[] = { h_ranges };
		int channels = 0;
		cv::MatND hist;

		cv::calcHist(&tmp, 1, &channels, cv::Mat(), hist, 1, &h_bins, ranges);

		_hist_views.emplace_back(hist);
	}
}

double TemplateMatching::compareHistH(cv::Mat test_img, int view_num)
{
	cvtColor(test_img, test_img, cv::COLOR_BGR2HSV);

	int h_bins = 45;
	float h_ranges[] = { 0, 180 };
	const float* ranges[] = {h_ranges};
	int channels = 0;
	cv::MatND test_hist;
	
	cv::calcHist(&test_img, 1, &channels, cv::Mat(), test_hist, 
		         1, &h_bins, ranges);
	
	return cv::compareHist(test_hist, _hist_views[view_num], 0);
}