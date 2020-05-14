#include "PanoramicImage.h"

PanoramicImage::PanoramicImage(cv::String path, float ratio) :
	_ratio{ratio}
{
	loadImages(path);
}

void PanoramicImage::panoramicImage()
{
	for(cv::Mat img : _input_imgs)
	{ 
		cv::Mat projected_img = PanoramicUtils::cylindricalProj(img, FOV / 2);
		_projected_imgs.push_back(projected_img);
	}

	cv::Ptr<cv::ORB> orb = cv::ORB::create();
	std::vector<std::vector<cv::KeyPoint>> keypoints;
	std::vector<cv::Mat> descriptors;

	for(cv::Mat img : _projected_imgs)
	{
		std::vector<cv::KeyPoint> kpts;
		cv::Mat desc;

		orb->detectAndCompute(img, cv::Mat(), kpts, desc);
	
		descriptors.push_back(desc);
		keypoints.push_back(kpts);
	}

	cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
	float min = std::numeric_limits<float>::max();
	std::vector<std::vector<cv::DMatch>> matches;

	for (int i=1; i<descriptors.size(); i++)
	{
		std::vector<cv::DMatch> match1;
		matcher->match(descriptors[i-1], descriptors[i], match1, cv::Mat());
		findMin(min, match1);
		matches.push_back(match1);
	}

	std::cout << LINE << std::endl;
	std::cout << "min: "<<min << std::endl;
	std::cout << "ratio: " << _ratio << std::endl;
	std::cout << LINE << std::endl;

	float threshold = _ratio*min;

	std::vector<cv::Point2f> translations;

	for (int i = 0; i < matches.size(); i++)
	{
		std::vector<cv::Point2f> pointsO;
		std::vector<cv::Point2f> pointsS;
		cv::Mat mask;

		for (int j = 0; j < matches[i].size(); j++)
		{
			if (matches[i][j].distance < threshold)
			{
				//indice del keypoint dell'immagine i-1 
				pointsO.push_back(keypoints[i][matches[i][j].queryIdx].pt);
			
				//indice del keypoint dell'immagine i
				pointsS.push_back(keypoints[i+1][matches[i][j].trainIdx].pt);
			}
		}

		cv::Mat H = cv::findHomography(pointsO, pointsS, mask, cv::RANSAC, threshold);
	
		cv::Point2f transl(0.0,0.0);

		int k = 0;
		for(; k<mask.rows; k++)
		{		
			if ((unsigned int)mask.at<uchar>(k))
			{
				transl.x += (pointsS[k].x - pointsO[k].x);
				transl.y += (pointsS[k].y - pointsO[k].y);
			}
		}

		transl.x /= k;
		transl.y /= k;

		translations.push_back(transl);
	}

	float  width_f = _projected_imgs[0].cols+0.0;
	for (int i = 1; i < _projected_imgs.size(); i++)
		width_f += (translations[i-1].x);

	int width = cvRound(width_f);

	
	for (const auto& img_r : _projected_imgs)
	{
		cv::equalizeHist(img_r, img_r);
	}

	cv::Mat panoramic_view(cv::Size(width, _projected_imgs[0].rows), _projected_imgs[0].type());
	int x1 = 0;
	int x2 = _projected_imgs[0].cols;
	cv::Mat img = panoramic_view(cv::Range(0, _projected_imgs[0].rows), cv::Range(x1, x2));
	_projected_imgs[0].copyTo(img);

	for (int i = 1; i < _projected_imgs.size(); i++)
	{
		x1 = x1+ cvRound(translations[i-1].x);
		x2 = x1 + _projected_imgs[i].cols ;
	
		cv::Mat img = panoramic_view(cv::Range(0, _projected_imgs[0].rows), cv::Range(x1, x2));
		_projected_imgs[i].copyTo(img);
	}

	cv::resize(panoramic_view, panoramic_view, cv::Size(width/6, _projected_imgs[0].rows/6));
	cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
	cv::imshow(window, panoramic_view);
	cv::waitKey(0);

}

void PanoramicImage::loadImages(cv::String path)
{
	std::vector<cv::String> imgs_names;
	cv::utils::fs::glob(path, PATTERN, imgs_names);

	for (cv::String name : imgs_names)
	{
		cv::Mat img = imread(name, cv::IMREAD_COLOR);
		_input_imgs.push_back(img);
	}
}


void PanoramicImage::findMin(float &min, std::vector<cv::DMatch> d_matches)
{
	for (cv::DMatch match : d_matches)
	{
		if (match.distance < min)
			min = match.distance;
	}
}