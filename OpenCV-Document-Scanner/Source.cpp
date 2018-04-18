#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "OpenCVMain.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat image0, image, gray, mask;
Mat img;
int ffillMode = 1;
int loDiff = 20, upDiff = 20;
int connectivity = 4;
int isColor = true;
OpenCVMain openCVMain;



static void onMouse(int event, int x, int y, int, void*)
{
	if (event != EVENT_LBUTTONDOWN)
		return;

	image0 = img.clone();
	resize(image0, image0, Size((int)((float)image0.size().width / 2), (int)((float)image0.size().height / 2)));
	Point seed = Point();
	seed.x = image0.size().width / 2;
	seed.y = image0.size().height / 2;
	Mat mask = openCVMain.getMask(image0, seed);
	//imshow("mask", mask);
	//waitKey(1);
	vector<Point2f> pointsRect = openCVMain.getRectanglePoints(mask);
	Mat dst;
	image0.copyTo(dst);
	for (int i = 0; i < pointsRect.size(); i++) {
		circle(dst, pointsRect.at(i), 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
	}
	//Mat dst = openCVMain.getFourPointsTransformation(image0, pointsRect);
	imshow("WithPoints", dst);
	waitKey(0);

}
int main(int argc, char** argv)
{
	// Enter path to image 
	// Click enter
	img = imread("20180310_008_jl.png");
	namedWindow("image", 0);
	setMouseCallback("image", onMouse, 0);
	cout << "Press a mouse button on image" << endl;
	while (true) {

		imshow("image", img);
		waitKey(0);
	}


	return 0;
}