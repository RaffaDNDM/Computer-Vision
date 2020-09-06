/**
	@file BestResults.hpp
	@brief Class for management of the results with higher score.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#ifndef BEST_RESULTS
#define BEST_RESULTS

#include "Project.hpp"

class Result {
public:
	/**
		@brief Constructor of Result Entry.
		@param pos position of the match in the image
		@param img_index index of the image on which the match is found
		@param mask_index index of the mask that matches for this result
		@param score score of the found match
	*/
	Result(cv::Point pos, int img_index, int mask_index, double score);

	/**
		@brief Get the position of the match of the mask in the image.
		@return position of the match
	*/
	cv::Point getPoint();

	/**
		@brief Get the image index of the image related to this result.
		@return image index on which the match is found
	*/
	int getImageIndex();

	/**
		@brief Get the mask index of the mask related to this result.
		@return mask index of the mask that matches for this result
	*/
	int getMaskIndex();

	/**
		@brief Get score of this result.
		@return score of the founf match
	*/
	double getScore();

private:
	//Position of the match in the image
	cv::Point _pos;
	
	//Index of the image on which the match is found
	int _img_index;
	
	//Index of the mask that matches for this result
	int _mask_index;
	
	//Score of the found match
	double _score;
};

class BestResults {
public:
	/**
		@brief Constructor of the best max_size results.
		@param max_size size of the buffer of the results
	*/
	BestResults(int max_size);

	/**
		@brief Create and insert a new result in the buffer.
		@param pos position of the match in the image
		@param img_index index of the image on which the match is found
		@param mask_index index of the mask that matches in position in the img_index image
		@param score score of the found match
	*/
	void insert(cv::Point pos, int img_index, int mask_index, double score);

	/**
		@brief Return the buffer of the best results.
		@return vector contatins the best max_size matches
	*/
	std::vector<Result> getBestResults();

private:
	/**
		@brief Get the score of the result with worst score in the buffer.
		@return value of the min score
	*/
	double getMinScore();

	/**
		@brief Get the score of the result with best score in the buffer.
		@return value of the max score
	*/
	double getMaxScore();

	//Buffer of best found matches
	std::vector<Result> _best_elements;
	
	//Default max size of the buffer
	int MAX_SIZE = 30;
};

#endif