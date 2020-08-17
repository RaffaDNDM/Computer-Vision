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

BestResults::BestResults()
{}

void BestResults::insert(cv::Point pos, int img_index, int mask_index, double score)
{
	//std::cout << "(" << pos.x << "," << pos.y << ")  " << img_index << "   " << mask_index << "   "<< score << std::endl;
	Result r(pos, img_index, mask_index, score);
	
	int i = 0;
	if(_best_elements.size()==0)
		_best_elements.emplace_back(r);
	else
	{
		if (_best_elements.size() == MAX_SIZE)
		{
			//No insertion of the new element
			if (r.getScore() <= getMinScore())
				return;
		}

		std::vector<Result>::iterator iter = _best_elements.begin();
		for (; iter != _best_elements.end(); ++iter)
		{
			if (score < (*iter).getScore())
			{
				iter = _best_elements.insert(iter, r);
				break;
			}
		}

		if (iter == _best_elements.end())
			_best_elements.emplace_back(r);
		
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