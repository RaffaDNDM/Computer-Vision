#define LINE "_______________________________________________\n"

namespace Dataset {
	enum class Type {
		CAN,
		DRILL,
		DUCK
	};

	enum class PatternIMG {
		MASKS,
		VIEWS,
		TEST,
		SET_IMAGES
	};

	const std::vector<cv::String> sub_folders = { "/models", "/models", "/test_images" };
	const std::vector<cv::String> patterns = { "mask*.png", "model*.png", "*.jpg" };
	const std::vector<cv::String> types = { "can", "driller", "duck" };
}