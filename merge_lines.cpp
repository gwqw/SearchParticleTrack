#include "merge_lines.h"

#include <algorithm>
#include <cmath>

using namespace std;

std::vector<cv::Vec4i> mergeLines(const std::vector<cv::Vec4i>& lines, double threshold) {
    /* calc slope
     * sort by slope
     * merge lines by slope
     */
    constexpr double PI = 3.14159265358979;
    vector<pair<double, vector<cv::Vec4i>::const_iterator>> slopes;
    slopes.reserve(lines.size());
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        const auto& line = *it;
        double slope = line[2] != line[0] ?
                atan((double(line[3]) - line[1]) / (line[2] - line[0])) :
                PI;
        slopes.emplace_back(slope, it);
    }
    sort(slopes.begin(), slopes.end(),
            [](const auto& lhs, const auto& rhs) {
                return lhs.first < rhs.first;
            }
    );
    slopes.erase(
        unique(slopes.begin(), slopes.end(),
            [threshold](const auto& lhs, const auto& rhs) {
                return abs(lhs.first - rhs.first) < threshold;
            }
        ), slopes.end()
    );

    std::vector<cv::Vec4i> res;
    res.reserve(slopes.size());
    for (const auto& [_, it] : slopes) {
        res.push_back(*it);
    }
    return res;
}

std::vector<cv::Vec4i> convertLinesTo2PointsLines(const std::vector<cv::Vec2f>& lines) {
    std::vector<cv::Vec4i> res;
    res.reserve(lines.size());
    for (const auto& line : lines) {
        float rho = line[0], theta = line[1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 - 1000*(-b));
        pt1.y = cvRound(y0 - 1000*(a));
        pt2.x = cvRound(x0 + 1000*(-b));
        pt2.y = cvRound(y0 + 1000*(a));
        res.emplace_back(pt1.x, pt1.y, pt2.x, pt2.y);
    }
    return res;
}