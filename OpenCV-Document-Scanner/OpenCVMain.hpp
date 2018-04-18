//
// Created by Daniel on 12.11.2017.
//

#ifndef RECEIPTSAVE_OPENCVMAIN_HPP
#define RECEIPTSAVE_OPENCVMAIN_HPP


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
class OpenCVMain
{
public:
    OpenCVMain();
    ~OpenCVMain();
    cv::Mat getFourPointsTransformation(cv::Mat src, std::vector<cv::Point2f> points);
	std::vector<cv::Point2f> getRectanglePoints(cv::Mat mask);
	cv::Mat getMask(cv::Mat src, cv::Point seedPoint);
private:
    struct less_than_key
    {
        inline bool operator() (const pair<int, double>& struct1, const pair<int, double>& struct2)
        {
            return (struct1.second > struct2.second);
        }
    };

	int ffillMode = 1;
	int connectivity = 4;
};


#endif //RECEIPTSAVE_OPENCVMAIN_HPP
