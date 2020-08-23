/**
	@file TemplateMatching.cpp
	@brief Implementation of class for template matching.
	@author Di Nardo Di Maio Raffaele 1204879
*/
#include "TemplateMatching.hpp"
#include "CannyDetector.hpp"

static std::mutex mutex;

TemplateMatching::TemplateMatching(cv::String path, Utility::Type dataset_type) :
	_dataset_type{ dataset_type },
	_param { Utility::canny_params[static_cast<int>(_dataset_type)] }
{
	if (path.back() == '\\' || path.back() == '/')
		loadImages(path + Utility::types[static_cast<int>(dataset_type)]);
	else
		loadImages(path + "/" + Utility::types[static_cast<int>(dataset_type)]);

	_fs.open(Utility::results_files_path + Utility::types[static_cast<int>(_dataset_type)] + "_results.txt", std::fstream::out);
}

void TemplateMatching::loadImages(cv::String path) 
{
	int i = 0;
	for (; i<static_cast<int>(Utility::PatternIMG::SET_IMAGES); i++)
	{
		std::vector<cv::String> imgs_names;
		cv::utils::fs::glob(path+ Utility::sub_folders[i], Utility::patterns[i], imgs_names);

		if (imgs_names.size() == 0)
			break;
		else
		{
			std::vector<cv::Mat> tmp_imgs;
			for (cv::String name : imgs_names)
			{
				cv::Mat img;
				if (i == static_cast<int>(Utility::PatternIMG::MASKS))
					img = imread(name, cv::IMREAD_GRAYSCALE);
				else
					img = imread(name, cv::IMREAD_COLOR);

				tmp_imgs.emplace_back(img);
			}

			_input_imgs.emplace_back(tmp_imgs);
		}
	}

	if (i < static_cast<int>(Utility::PatternIMG::SET_IMAGES))
		throw InputIMGException();

	mutex.lock();
	stateAcquisitionImages(path);
	mutex.unlock();
}



void TemplateMatching::stateAcquisitionImages(cv::String path)
{
	std::cout << std::endl << Utility::bold_colors[static_cast<int>(_dataset_type)] << path << std::endl;
	std::cout << LINE;
	std::cout << "Dataset of " << Utility::types[static_cast<int>(_dataset_type)] << DEFAULT << std::endl;
	std::cout << "Number of masks images:   " << Utility::colors[static_cast<int>(_dataset_type)]
		      << _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)].size() << DEFAULT << std::endl;
	std::cout << "Number of views images:   " << Utility::colors[static_cast<int>(_dataset_type)]
			  << _input_imgs[static_cast<int>(Utility::PatternIMG::VIEWS)].size() << DEFAULT << std::endl;
	std::cout << "Number of test images:    " << Utility::colors[static_cast<int>(_dataset_type)] 
		      << _input_imgs[static_cast<int>(Utility::PatternIMG::TEST)].size() << std::endl;
	std::cout << Utility::bold_colors[static_cast<int>(_dataset_type)] << LINE << DEFAULT << std::endl;
}

void TemplateMatching::cannyDetection()
{
	GammaTransform g(Utility::gammas[static_cast<int>(_dataset_type)]);

	for (auto& img : _input_imgs[static_cast<int>(Utility::PatternIMG::VIEWS)])
	{
		cv::Mat tmp = img.clone();
		g.computeTransform(img, tmp);
		CannyDetector cd(img, _dataset_type, _param.threshold_template, _param.threshold_template - 30.0);
		cd.detect();

		_canny_views.emplace_back(cd.getResult());
	}

	computeHist(_input_imgs[static_cast<int>(Utility::PatternIMG::VIEWS)]);
}

void TemplateMatching::match()
{
	cannyDetection();

	int count_white=0;
	int img_index = 0;
	for (auto img : _input_imgs[static_cast<int>(Utility::PatternIMG::TEST)])
	{
		double abs_max = 0.0;
		int max_index = 0;
		cv::Point max_pos;

		int mask_index = 0;
		cv::Mat img_copy = img.clone();
		BestResults r1(50), r2(10);
		CannyDetector cd(img_copy, _dataset_type, _param.threshold_test, _param.threshold_test - 40.0);
		cd.detect();

		for (auto filter : _canny_views)
		{
			cv::Mat result(cv::Size(img_copy.rows - filter.rows + 1, img_copy.cols - filter.cols + 1), CV_32F);
			double min_score, max_score;
			cv::Point min_point, max_point;
			cv::matchTemplate(cd.getResult(), filter, result, cv::TM_CCOEFF);
			cv::normalize(result, result);
			cv::minMaxLoc(result, &min_score, &max_score, &min_point, &max_point);
			r1.insert(max_point, img_index, mask_index, max_score);

			mask_index++;
		}

		for (auto res : r1.getBestResults())
		{
			cv::Mat detected_patch = img(cv::Range(res.getPoint().y,
				res.getPoint().y + _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)][res.getMaskIndex()].rows),
				cv::Range(res.getPoint().x,
					res.getPoint().x + _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)][res.getMaskIndex()].cols))
				.clone();

			cv::Mat mask = _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)][res.getMaskIndex()].clone();

			double count = 0.0;
			double count_w = 0.0;
			for (int i = 0; i < mask.rows; i++)
				for (int j = 0; j < mask.cols; j++)
					if (mask.at<uchar>(i, j) < 50)
						detected_patch.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);

			double score = compareHistH(detected_patch, res.getMaskIndex());
			r2.insert(res.getPoint(), res.getImageIndex(), res.getMaskIndex(), 100.0*res.getScore() + score);
		}

		printBestMatch(r2, img);

		img_index++;
	}

	_fs.close();
}

void TemplateMatching::printBestMatch(BestResults best_results, cv::Mat img)
{
	std::vector<Result> results = best_results.getBestResults();
	int index = 0;

	_fs << "test" << std::to_string(results[0].getImageIndex()) << ".jpg";

	for (int k = results.size() - 1; k >= 0; k--)
	{
		cv::Mat mask_result=img.clone();

		for (int i = 0; i < _canny_views[results[k].getMaskIndex()].rows; i++)
		{
			for (int j = 0; j < _canny_views[results[k].getMaskIndex()].cols; j++)
			{
				if (_canny_views[results[k].getMaskIndex()].at<uchar>(i, j) > 100)
					mask_result.at<cv::Vec3b>(results[k].getPoint().y + i, results[k].getPoint().x + j) = Utility::MASK_CANNY;
			}
		}

		cv::putText(mask_result,
			"mask " + std::to_string(results[k].getMaskIndex()),
			cv::Point(40, 40), // Coordinates
			cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale. 2.0 = 2x bigger
			Utility::MASK_NAME); // BGR Color

		cv::imwrite(Utility::output_path + Utility::types[static_cast<int>(_dataset_type)] +
			"/result" + std::to_string(results[0].getImageIndex()) +"_"+
			std::to_string(index)+".jpg", mask_result);

		index++;

		_fs << " model" << results[k].getMaskIndex() << ".png " << results[k].getPoint().x << " " << results[k].getPoint().y;
	}

	_fs << std::endl;

	mutex.lock();
	
	for(int k=results.size()-1; k>=0; k--)
	{
		std::cout << Utility::colors[static_cast<int>(_dataset_type)] << "image " << results[k].getImageIndex() << ":" DEFAULT << std::setw(8)
			<< std::right << " mask" + std::to_string(results[k].getMaskIndex()) << Utility::colors[static_cast<int>(_dataset_type)]
			<< " >>> " << DEFAULT << std::setw(11) << std::left << results[k].getScore() << Utility::colors[static_cast<int>(_dataset_type)]
			<< " in " << DEFAULT << "(" << results[k].getPoint().y << "," << results[k].getPoint().x << ")" << std::endl;
	}

	mutex.unlock();
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