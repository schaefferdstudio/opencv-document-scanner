#ifndef OPENCVOPERATIONS_HPP
#define OPENCVOPERATIONS_HPP
#define PADDING 10
#include <opencv2/opencv.hpp>

using namespace std;
class OpenCVOperations
{
public:
	static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	static double distance_to_Line(cv::Point line_start, cv::Point line_end, cv::Point point);
	static vector<cv::Point> getPointsFromLine(cv::Vec2f line);
	static bool getIntersectionPoint(vector<cv::Point> a, vector<cv::Point> b, vector<cv::Point2f>* returnPoints);
	static double cross(cv::Point v1, cv::Point v2);
	static bool checkPointInImage(cv::Point point, cv::Mat image);
	static cv::Point pointFromLineAtX(vector<cv::Point> points, int xVal);
	static cv::Point pointFromLineAtY(vector<cv::Point> points, int yVal);
private:
};
#endif

