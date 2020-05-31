/**
	@file Test.h
	@brief Header of source file that changes input image.
	@author Di Nardo Di Maio Raffaele
*/

#ifndef TEST
#define TEST

#define GREEN 0  
#define BLUE 1
#define RED 2
#define NUM_RGB 3
#define TIMEOUT 2000

#include"HelloWorld.h"

/**
	@brief Shows an image in a window.
	@param filename name of the image file with extension
*/
void show_image(char*);

/**
	@brief Shows an image, converting it to BW, in a window.
	@param filename name of the image file with extension
*/
void show_imageBW(char*);

/**
	@brief Shows an image in a window looking only to one channel.
	@param filename name of the image file with extension
	@param channel (BLUE=0, GREEN=1, RED=2)
*/
void component_select(char*, int);

/**
	@brief Shows an image in a window, converting black to white inside of it.
	@param filename name of the image file with extension
*/
void b2w(char*);

/**
	@brief Shows an image in a window, converting white to black inside of it.
	@param filename name of the image file with extension
*/
void w2b(char*);

/**
	@brief Shows a diagonal gray variation in a 256px*256px window.
*/
void gray_variation_diagonal(void);

/**
	@brief Shows a horizontal gray variation in a 256px*256px window.
*/
void gray_variation_horizontal(void);

/**
	@brief Shows a vertival gray variation in a 256px*256px window.
*/
void gray_variation_vertical(void);

#endif 