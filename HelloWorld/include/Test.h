#ifndef TEST
#define TEST

#define GREEN 0  
#define BLUE 1
#define RED 2
#define NUM_RGB 3
#define TIMEOUT 2000

#include <opencv2/highgui.hpp>

/**
	Shows an image in a window
	/param filename name of the image file with extension
*/
void show_image(char*);

/**
	Shows an image, converting it to BW, in a window
	/param filename name of the image file with extension
*/
void show_imageBW(char*);

/**
	Shows an image in a window looking only to one channel
	/param filename name of the image file with extension
	/param channel (BLUE=0, GREEN=1, RED=2)
*/
void component_select(char*, int);

/**
	Shows an image in a window, converting black to white inside of it
	/param filename name of the image file with extension
*/
void b2w(char*);

/**
	Shows an image in a window, converting white to black inside of it
	/param filename name of the image file with extension
*/
void w2b(char*);

/**
	Shows a diagonal gray variation in a 256px*256px window
*/
void gray_variation_diagonal(void);

/**
	Shows a horizontal gray variation in a 256px*256px window
*/
void gray_variation_horizontal(void);

/**
	Shows a vertival gray variation in a 256px*256px window
*/
void gray_variation_vertical(void);

#endif 