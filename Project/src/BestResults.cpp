/**
	@file BestResults.cpp
	@brief Implementation of class for management of the results with higher score.
	@author Di Nardo Di Maio Raffaele 1204879
*/

#include "BestResults.hpp"

Result::Result(cv::Point pos, int img_index, int mask_index, double score):
	_pos{pos},
	_img_index{img_index},
	_mask_index{mask_index},
	_score{score}
{}

cv::Point Result::getPoint() 
{
	return _pos;
}

int Result::getImageIndex()
{
	return _img_index;
}

int Result::getMaskIndex()
{
	return _mask_index;
}

double Result::getScore()
{
	return _score;
}

BestResults::BestResults(int max_size) :
	MAX_SIZE{ max_size }
{}

void BestResults::insert(cv::Point pos, int img_index, int mask_index, double score)
{
	//Creation of the Result entity
	Result r(pos, img_index, mask_index, score);
	
	//Insertion of the element in the buffer
	int i = 0;
	if(_best_elements.size()==0)
		_best_elements.emplace_back(r);
	else
	{
		if (_best_elements.size() == MAX_SIZE)
		{
			//Full buffer (No insertion of the new element)
			if (r.getScore() <= getMinScore())
				return;
		}

		//Find correct position in the buffer for the new insertion
		std::vector<Result>::iterator iter = _best_elements.begin();
		for (; iter != _best_elements.end(); ++iter)
		{
			if (score < (*iter).getScore())
			{
				iter = _best_elements.insert(iter, r);
				break;
			}
		}

		//This new result has the highest score (best one)
		if (iter == _best_elements.end())
			_best_elements.emplace_back(r);
		
		//I overcome the MAX SIZE inserting the new result
		if (_best_elements.size() == (MAX_SIZE + 1))
			_best_elements.erase(_best_elements.begin());
	}
}

std::vector<Result> BestResults::getBestResults()
{
	return _best_elements;
}

double BestResults::getMinScore()
{
	return _best_elements.front().getScore();
}

double BestResults::getMaxScore()
{
	return _best_elements.back().getScore();
}