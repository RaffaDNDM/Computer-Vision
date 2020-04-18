#include "Test.h"
#include <opencv2/highgui.hpp>
#include "HelloWorld.h"
#include <cstring>

int main(int argc, char* argv[])
{
    show_image(argv[1]);
    show_imageBW(argv[1]);

    component_select(argv[1], RED);
    component_select(argv[1], GREEN);
    component_select(argv[1], BLUE);

    b2w(argv[1]);
    w2b(argv[1]);

    gray_variation_diagonal();
    gray_variation_horizontal();
    gray_variation_vertical();

    return 0;
}
