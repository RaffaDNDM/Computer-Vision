#include "BestResults.hpp"

Result::Result(cv::Point pos, int img_index, int mask_index, double score):
	_pos{pos},
	_img_index{img_index},
	_mask_index{mask_index},
	_score{score}
{

}

cv::Point Result::getPoint() 
{

}

int Result::getImageIndex()
{

}

int Result::getMaskIndex()
{

}

double Result::getScore()
{

}

BestResults::BestResults()
{}

void BestResults::insert(cv::Point pos, int img_index, int mask_index, double score)
{
	Result r(pos, img_index, mask_index, score);
	
	int i = 0;
	if (_size == MAX_SIZE)
	{
					
	}

	if (i < _size)
	{
		for (std::vector<Result>::iterator iter = _best_elements.begin(); iter != _best_elements.end(); ++iter)
		{
			if (score > (*iter).getScore())
			{
				(*iter) = r;

				break;
			}
		}
	}
	else
	{
		_min_value = _max_value = score;
		_best_elements.emplace_back(r);
		_size++;
	}
}

std::vector<Result> BestResults::getBestResults()
{

}