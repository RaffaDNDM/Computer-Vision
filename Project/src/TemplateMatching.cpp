/**
	@file TemplateMatching.cpp
	@brief Implementation of class for template matching.
	@author Di Nardo Di Maio Raffaele 1204879
*/
#include "TemplateMatching.hpp"
#include "CannyDetector.hpp"

static std::mutex mutex;

TemplateMatching::TemplateMatching(cv::String input_path, cv::String results_path,
								   cv::String output_path, Utility::Type dataset_type,
								   bool distance_transform) :
	_dataset_type{ dataset_type },
	_param{ Utility::canny_params[static_cast<int>(_dataset_type)] },
	_distance_transform{_distance_transform}
{
	//Definition of path for actual dataset looking to termination character	
	loadImages(pathFormat(input_path) + Utility::types[static_cast<int>(dataset_type)]);

	//If specified by user, I use the specified path otherwise the default one
	if (results_path != "")
		_results_path = pathFormat(results_path);

	_fs.open(_results_path + Utility::types[static_cast<int>(_dataset_type)] + "_results.txt", std::fstream::out);

	_output_path = (output_path != "")? pathFormat(output_path) : output_path;
	
}

cv::String TemplateMatching::pathFormat(cv::String path)
{
	if (path.back() != '\\' && path.back() != '/')
		return path + "/";
	else
		return path;
}

void TemplateMatching::loadImages(cv::String path) 
{
	//Insertion of all the input images
	int i = 0;
	for (; i<static_cast<int>(Utility::PatternIMG::SET_IMAGES); i++)
	{
		std::vector<cv::String> imgs_names;
		cv::utils::fs::glob(path+ Utility::sub_folders[i], Utility::patterns[i], imgs_names);

		/*
			Problems with detection of elements with this pattern in the actual subfolder
			(can/, driller/ or duck/). Folder or files inside it don't exist.
		*/
		if (imgs_names.size() == 0)
			break;
		else //Images found
		{
			std::vector<cv::Mat> tmp_imgs;
			
			//Store images
			for (cv::String name : imgs_names)
			{
				cv::Mat img;
				
				/*
					MASKS = grayscale images
					VIEWS = RGB images
					TEST = RGB images
				*/
				if (i == static_cast<int>(Utility::PatternIMG::MASKS))
					img = imread(name, cv::IMREAD_GRAYSCALE);
				else
					img = imread(name, cv::IMREAD_COLOR);

				tmp_imgs.emplace_back(img);
			}

			_input_imgs.emplace_back(tmp_imgs);
		}
	}

	//Problem detected in loading phase
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
	//Canny detection over all the view images
	GammaTransform g(Utility::gammas[static_cast<int>(_dataset_type)]);

	for (auto& img : _input_imgs[static_cast<int>(Utility::PatternIMG::VIEWS)])
	{
		cv::Mat tmp = img.clone();
		g.computeTransform(img, tmp);
		CannyDetector cd(img, _dataset_type, _param.threshold_template - 30.0, _param.threshold_template);
		cd.detect();

		_canny_views.emplace_back(cd.getResult());
	}

	//Histogram compution over all the view images
	computeHistViews(_input_imgs[static_cast<int>(Utility::PatternIMG::VIEWS)]);
}

void TemplateMatching::match(cv::Mat canny_img, int img_index, BestResults& r)
{
	int mask_index = 0;

	//Compute template matchin for each view with detected edges
	for (auto filter : _canny_views)
	{
		cv::Mat result(cv::Size(canny_img.rows - filter.rows + 1, canny_img.cols - filter.cols + 1), CV_32F);
		double min_score, max_score;
		cv::Point min_point, max_point;
		
		if (_distance_transform)
		{
			//Distance transform
			cv::Mat dist_img;
			cv::distanceTransform(canny_img, dist_img, cv::DIST_L2, 3);
			cv::normalize(dist_img, dist_img, 0.0, 1.0, cv::NORM_MINMAX);

			cv::Mat tmp;
			filter.convertTo(tmp, CV_32F);

			//Template Matching using Correlation Coefficient 
			cv::matchTemplate(dist_img, tmp, result, cv::TM_CCOEFF);
		}
		else
		{
			//Template Matching using Correlation Coefficient 
			cv::matchTemplate(canny_img, filter, result, cv::TM_CCOEFF);
		}

		//Normalization of results
		cv::normalize(result, result);
		//Find min and max elements in result matrix
		cv::minMaxLoc(result, &min_score, &max_score, &min_point, &max_point);
		//Insert result corresponding to the max position found for this view
		r.insert(max_point, img_index, mask_index, max_score);

		mask_index++;
	}
}

void TemplateMatching::refinement(cv::Mat img, BestResults& r1, BestResults& r2)
{
	//Scan all the best result previously computed
	for (auto res : r1.getBestResults())
	{
		//Compute subwindow in the image of size of the corresponding view
		cv::Mat detected_patch = img(cv::Range(res.getPoint().y,
			res.getPoint().y + _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)][res.getMaskIndex()].rows),
			cv::Range(res.getPoint().x,
				res.getPoint().x + _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)][res.getMaskIndex()].cols))
			.clone();

		//Filter the subwindow using the mask related to the best view
		cv::Mat mask = _input_imgs[static_cast<int>(Utility::PatternIMG::MASKS)][res.getMaskIndex()].clone();

		for (int i = 0; i < mask.rows; i++)
			for (int j = 0; j < mask.cols; j++)
				if (mask.at<uchar>(i, j) < 50)
					detected_patch.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);

		
		//Compare HUE histograms of subwindow and related view 
		double score = compareHistH(detected_patch, res.getMaskIndex());

		/*
			Insert the result with the following score:
			new_score = score_match*100.0 + score_hist 

			score_match: score obtained by Template Matching
			 score_hist: score obtained by HUE Histogram Comparison
		*/
		r2.insert(res.getPoint(), res.getImageIndex(), res.getMaskIndex(), 100.0 * res.getScore() + score);
	}
}

void TemplateMatching::match()
{
	//Computation of histograms and canny detection over all the view images
	cannyDetection();

	//Template matching over all the test images
	int img_index = 0;
	for (auto img : _input_imgs[static_cast<int>(Utility::PatternIMG::TEST)])
	{
		double abs_max = 0.0;
		int max_index = 0;
		cv::Point max_pos;

		BestResults r2(10);
		CannyDetector cd(img.clone(), _dataset_type, _param.threshold_test-40.0, _param.threshold_test);
		cd.detect();

		if (_distance_transform)
		{
			//Template matching using Chamfer distance
			match(cd.getResult(), img_index, r2);
		}
		else
		{
			BestResults r1(50);
			//Template matching based on Canny detection
			match(cd.getResult(), img_index, r1);
			//Refinement based on histogram comparison
			refinement(img, r1, r2);
		}

		//Print results on screen and in relative file
		printBestMatch(r2, img);

		img_index++;
	}

	_fs.close();
}


void TemplateMatching::printBestMatch(BestResults best_results, cv::Mat img)
{
	std::vector<Result> results = best_results.getBestResults();
	int index = 0;

	//Print name of image on the related result file
	_fs << "test" << std::to_string(results[0].getImageIndex()) << ".jpg";

	//Analyze the best results of template matching on img
	for (int k = results.size() - 1; k >= 0; k--)
	{
		if (_output_path != "")
		{
			cv::Mat mask_result = img.clone();

			//Print the edges detected on the view on the image
			for (int i = 0; i < _canny_views[results[k].getMaskIndex()].rows; i++)
			{
				for (int j = 0; j < _canny_views[results[k].getMaskIndex()].cols; j++)
				{
					if (_canny_views[results[k].getMaskIndex()].at<uchar>(i, j) > 100)
						mask_result.at<cv::Vec3b>(results[k].getPoint().y + i, results[k].getPoint().x + j) = Utility::MASK_CANNY;
				}
			}

			//Add the name of the mask to the result image
			cv::putText(mask_result,
				"mask " + std::to_string(results[k].getMaskIndex()), //String
				cv::Point(40, 40), //Coordinates
				cv::FONT_HERSHEY_COMPLEX_SMALL, //Font
				1.0, //Scale
				Utility::MASK_NAME); //BGR Color

			//Store the result image on the disk
			cv::imwrite(_output_path + Utility::types[static_cast<int>(_dataset_type)]
				+ std::to_string(results[0].getImageIndex()) + "_" +
				std::to_string(results.size() - 1 - k) + ".jpg", mask_result);
		}

		index++;

		//Print the name of model in the result file
		_fs << " model" << results[k].getMaskIndex() << ".png " << results[k].getPoint().x << " " << results[k].getPoint().y;
	}

	_fs << std::endl;


	//Print result on terminal
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

void TemplateMatching::computeHistViews(std::vector<cv::Mat> imgs)
{
	//Compute histogram of each image in the vector
	for(auto img : imgs)
	{
		//Histogram computation
		cv::MatND hist=computeHist(img);

		//Add computed histogram to the vector of histograms of the views images
		_hist_views.emplace_back(hist);
	}
}

double TemplateMatching::compareHistH(cv::Mat test_img, int view_num)
{
	//Compute HUE histogram
	cv::MatND test_hist = computeHist(test_img);
	
	//Compare computed histogram with histogram of view image
	return cv::compareHist(test_hist, _hist_views[view_num], 0);
}

cv::MatND TemplateMatching::computeHist(cv::Mat img)
{
	//Convert img from RGB space to HSV
	cv::Mat tmp;
	cvtColor(img, tmp, cv::COLOR_BGR2HSV);

	//Compute the histogram
	int h_bins = 45;
	float h_ranges[] = { 0, 180 };
	const float* ranges[] = { h_ranges };
	int channels = 0;
	cv::MatND hist;

	cv::calcHist(&tmp, 1, &channels, cv::Mat(), hist, 1, &h_bins, ranges);

	return hist;
}