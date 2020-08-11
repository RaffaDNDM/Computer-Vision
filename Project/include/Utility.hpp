#ifndef UTILITY
#define UTILITY

#define LINE "_______________________________________________\n"
#define BLUE "\033[0;34m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define MAGENTA "\033[0;35m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_RED "\033[1;31m"
#define BOLD_YELLOW "\033[1;33m"
#define DEFAULT "\033[0m"

namespace Dataset
{
	//Dataset type
	enum class Type
	{
		CAN,
		DRILLER,
		DUCK
	};

	//Pattern type
	enum class PatternIMG
	{
		MASKS,
		VIEWS,
		TEST,
		SET_IMAGES
	};

	//Dataset type
	enum class ColorSpace
	{
		B,
		G,
		R
	};

	const std::vector<cv::String> sub_folders = { "/models", "/models", "/test_images" };
	const std::vector<cv::String> patterns = { "mask*.png", "model*.png", "*.jpg" };
	const std::vector<cv::String> types = { "can", "driller", "duck" };
	const std::vector<cv::String> bold_colors = { BOLD_BLUE, BOLD_RED, BOLD_GREEN };
	const std::vector<cv::String> colors = { BLUE, RED, GREEN };
}

#endif