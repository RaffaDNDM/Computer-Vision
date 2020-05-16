#include "PanoramicImage.h"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order" << std::endl;
		std::cerr << "       <input_images_folder>     <1|2>" << std::endl;
		std::cerr << " 1 = ORB   2 = SIFT  "<< std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}
	
	std::string orb = "1";
	std::string sift = "2";
	bool isORB;

	if (argv[2] == orb)
		isORB = true;
	else if (argv[2] == sift)
		isORB = false;
	else
		return 1;

	PanoramicImage pi(argv[1]);
	pi.panoramicImage(isORB);
}