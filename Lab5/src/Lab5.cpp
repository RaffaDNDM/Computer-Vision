#include "PanoramicImage.h"

int main(int argc, char** argv)
{
	if (argc != 2)
		std::cerr<<"Few argument"<<std::endl;

	for (float ratio=8.0; ratio>3.0; ratio-=0.5)
	{
		PanoramicImage pi(argv[1], ratio);
		pi.panoramicImage();
	}
}