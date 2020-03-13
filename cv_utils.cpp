#include "cv_utils.h"

#include <stdexcept>
#include <cmath>
#include <iostream>

using namespace std;

ColorGenerator::ColorGenerator(int color_num)
    : color_num(color_num)
{
    if (color_num <= 0) {
        throw invalid_argument("Color count must be positive number");
    }
    double divisor = cbrt(color_num);
    step = static_cast<int>(255 / divisor);
    r = -step;
}

cv::Scalar ColorGenerator::getNextColor() {
    constexpr int MAX_COLOR = 256;
    r += step;
    if (r >= MAX_COLOR) {
        r = 0;
        g += step;
    }
    if (g >= MAX_COLOR) {
        g = 0;
        b += step;
    }
    if (b >= MAX_COLOR) {
        b = 0;
    }
    return cv::Scalar(255-b, 255-g, 255-r);
}
