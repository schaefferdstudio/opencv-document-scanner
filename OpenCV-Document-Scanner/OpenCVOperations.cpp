#include "OpenCVOperations.hpp"
#include "OpenCVOperations.hpp"

using namespace std;
using namespace cv;

double OpenCVOperations::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


double OpenCVOperations::distance_to_Line(cv::Point line_start, cv::Point line_end, cv::Point point)
{
	double normalLength = hypot(line_end.x - line_start.x, line_end.y - line_start.y);
	double distance = (double)((point.x - line_start.x) * (line_end.y - line_start.y) - (point.y - line_start.y) * (line_end.x - line_start.x)) / normalLength;
	return distance;
}

vector<Point> OpenCVOperations::getPointsFromLine(Vec2f line) {

	vector<Point> points;
	float rho = line[0];
	float theta = line[1];
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	Point pt1(cvRound(x0 + 1000 * (-b)),
		cvRound(y0 + 1000 * (a)));
	Point pt2(cvRound(x0 - 1000 * (-b)),
		cvRound(y0 - 1000 * (a)));
	points.push_back(pt1);
	points.push_back(pt2);
	return points;
}


double OpenCVOperations::cross(Point v1, Point v2) {
	return v1.x*v2.y - v1.y*v2.x;
}

bool OpenCVOperations::checkPointInImage(cv::Point point, cv::Mat image) {
	if (point.x > 0 && point.y > 0) {
		if (point.x < image.size().width && point.y < image.size().height)
			return true;
	}
	return false;

}


bool OpenCVOperations::getIntersectionPoint(vector<Point> a, vector<Point> b, vector<Point2f>* returnPoints) {
	Point2f p = a.at(0);
	Point2f q = b.at(0);
	Point2f r ;
    Point2f s;
    r = a.at(1) - a.at(0);
	s = b.at(1) - b.at(0);

	if (cross(r, s) == 0) { return false; }

	double t = cross(q - p, s) / cross(r, s);

	Point2f ptr = p + t*r;
	returnPoints->push_back(ptr);
	return true;
}

Point OpenCVOperations::pointFromLineAtX(vector<Point> points, int xVal) {
	double m = (double)(points.at(1).y - points.at(0).y) / (double)(points.at(1).x - points.at(0).x);
	Point newPoint;
	newPoint.y = m *  (double)(xVal - points.at(0).x) + (double)points.at(0).y;
	newPoint.x = xVal;
	//circle(cdst, newPoint, 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
	return newPoint;
}

Point OpenCVOperations::pointFromLineAtY(vector<Point> points, int yVal) {
	double m = (double)(points.at(1).y - points.at(0).y) / (double)(points.at(1).x - points.at(0).x);
	Point newPoint;
	newPoint.y = yVal;
	newPoint.x = (double)(yVal - points.at(0).y) / m + points.at(0).x;
	//circle(cdst, newPoint, 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
	return newPoint;
}