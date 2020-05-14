#include "PanoramicImage.h"

int main(int argc, char** argv)
{
	if (argc != 2)
		std::cerr<<"Few argument"<<std::endl;

		PanoramicImage pi(argv[1]);
		pi.panoramicImage();
}