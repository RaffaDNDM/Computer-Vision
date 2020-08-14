#include "Project.hpp"

#ifndef BEST_RESULTS
#define BEST_RESULTS

class Result {
public:
	Result(cv::Point pos, int img_index, int mask_index, double score);

	cv::Point getPoint();

	int getImageIndex();

	int getMaskIndex();

	double getScore();

private:
	cv::Point _pos;
	int _img_index;
	int _mask_index;
	double _score;
};

class BestResults {
public:
	BestResults();

	void insert(cv::Point pos, int img_index, int mask_index, double score);

	std::vector<Result> getBestResults();

private:
	int _size = 0;
	std::vector<Result> _best_elements;
	const int MAX_SIZE = 10;
	double _min_value = -1;
	double _max_value = -1;
};

#endif