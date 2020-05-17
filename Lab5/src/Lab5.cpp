#include "PanoramicImage.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << LINE << std::endl;
		std::cerr << "You need to insert, as command line arguments, in order" << std::endl;
		std::cerr << "       <input_images_folder>     <1|2>" << std::endl;
		std::cerr << " 1 = ORB   2 = SIFT  "<< std::endl;
		std::cerr << LINE << std::endl;
		return 1;
	}
	
	bool isORB=true;

	PanoramicImage pi(argv[1]);
	pi.panoramicImage(isORB);
}