#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Vec4i> mergeLines(const std::vector<cv::Vec4i>& lines, double threshold);

std::vector<cv::Vec4i> convertLinesTo2PointsLines(const std::vector<cv::Vec2f>& lines);