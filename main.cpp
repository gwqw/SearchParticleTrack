#include <iostream>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "cv_utils.h"
#include "merge_lines.h"

using namespace std;

int main( int argc, char** argv ) {
    if (argc < 3) {
        cout << "Hough line detect\n"
                "Usage: " << argv[0] <<" <imagename> <method>"
                   "[<rho> <theta> [<threshold> [<minLineLength> <maxLineGap>]]]\n"
                   "method: 0 - standard, 1 - prob\n"
                   "rho: rho threshold in pixels\n"
                   "theta: theta threshold in degrees\n"
                   "threshold: threshold in pixels in line\n";
        return -1;
    }
    string img_name{argv[1]};
    int method = stol(argv[2]);
    double rho_res = 1, theta_res = CV_PI / 180;
    int threshold = 80;
    double minLineLength = 30, maxLineGap = 10;
    try {
        if (argc >= 5) {
            rho_res = stod(argv[3]);
            theta_res = CV_PI / 180 * stod(argv[4]);
        }
        if (argc >= 6) {
            threshold = stoi(argv[5]);
        }
        if (argc >= 8) {
            minLineLength = stod(argv[6]);
            maxLineGap = stod(argv[7]);
        }

        // read image
        cv::Mat src;
        src = cv::imread(img_name, 1);
        if (src.empty()) {
            cout << "Cannot load " << img_name << endl;
            return -1;
        }

        // image conversion
        cv::Mat image, dst, color_dst, color_dst_merged;
        cv::cvtColor(src, image, cv::COLOR_BGR2GRAY);
        //cv::GaussianBlur(image, image, cv::Size(5,5), 0, 0);

        // search lines
        Canny(image, dst, 50, 200, 3);
        cv::cvtColor(dst, color_dst, cv::COLOR_GRAY2BGR);
        cv::cvtColor(dst, color_dst_merged, cv::COLOR_GRAY2BGR);
        vector<cv::Vec4i> lines = [=]() {
            vector<cv::Vec4i> lines;
            if (method == 1) {
                cv::HoughLinesP(dst, lines,
                                rho_res, theta_res,               // rho and theta resolution
                                threshold,                        // pixels threshold
                                minLineLength, maxLineGap
                );
            } else if (method == 0) {
                vector<cv::Vec2f> lines_param;
                cv::HoughLines(dst, lines_param,
                               rho_res, theta_res,               // rho and theta resolution
                               threshold                         // pixels threshold
                );
                lines = convertLinesTo2PointsLines(lines_param);
            } else {
                throw invalid_argument("unknown line search method");
            }
            return lines;
        }();

        // merge and output lines
        cout << "Found " << lines.size() << " lines" << endl;
        {
            ColorGenerator color_gen(lines.size());
            for (const auto &line : lines) {
                auto color = color_gen.getNextColor();
                cv::line(
                        color_dst,
                        cv::Point(line[0], line[1]), // from
                        cv::Point(line[2], line[3]), // to
                        color,
                        2,                                   // thickness
                        cv::LINE_AA                     // line type
                );
            }
        }
        lines = mergeLines(lines, theta_res);
        cout << "After merge:  " << lines.size() << " lines" << endl;
        {
            ColorGenerator color_gen(lines.size());
            for (const auto &line : lines) {
                auto color = color_gen.getNextColor();
                cv::line(
                        color_dst_merged,
                        cv::Point(line[0], line[1]), // from
                        cv::Point(line[2], line[3]), // to
                        color,
                        2,                                   // thickness
                        cv::LINE_AA                     // line type
                );
            }
        }

        cv::imshow("original", image);
        cv::imshow("Hough Lines", color_dst);
        cv::imshow("Hough Merged Lines", color_dst_merged);
        cv::waitKey(0);
        return 0;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
};
