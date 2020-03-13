#pragma once

#include <opencv2/opencv.hpp>

class ColorGenerator {
public:
    explicit ColorGenerator(int color_num);
    cv::Scalar getNextColor();
private:
    int color_num;
    int step;
    int r = 0, g = 0, b = 0;
};

