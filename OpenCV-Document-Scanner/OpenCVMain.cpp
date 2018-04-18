//
// Created by Daniel on 12.11.2017.
//

#include "OpenCVMain.hpp"
#include "OpenCVOperations.hpp"
using namespace cv;


OpenCVMain::OpenCVMain()
{
}


OpenCVMain::~OpenCVMain()
{
}








Mat OpenCVMain::getFourPointsTransformation(Mat src, vector<Point2f> points) {
    Point2f* src_vertice = &points[0];

    double widthA = norm(Mat(src_vertice[0]), Mat(src_vertice[1]));
    double widthB = norm(Mat(src_vertice[2]), Mat(src_vertice[3]));
    double heightA = norm(Mat(src_vertice[0]), Mat(src_vertice[3]));
    double heightB = norm(Mat(src_vertice[1]), Mat(src_vertice[2]));
    double maxWidth = max(widthA, widthB);
    double maxHeight = max(heightA, heightB);

    Point2f dst_vertices[4];
    dst_vertices[0] = Point(0, 0);
    dst_vertices[1] = Point(maxWidth - 1, 0); // Bug was: had mistakenly switched these 2 parameters
    dst_vertices[2] = Point(maxWidth - 1, maxHeight - 1);
    dst_vertices[3] = Point(0, maxHeight - 1);

    Mat M = getPerspectiveTransform(src_vertice, dst_vertices);
    Mat wraped;
    warpPerspective(src, wraped, M, Size(maxWidth, maxHeight));
    return wraped;

}





Mat OpenCVMain::getMask(Mat src, Point seedPoint) {
	Mat hChannel;
	//cvtColor(src, imgCopy, CV_BGR2HSV);
	//vector<Mat> channels;
	//split(imgCopy, channels);
	//GaussianBlur(channels[0], hChannel, Size(23, 23), 0, 0);	
	cvtColor(src, hChannel, CV_BGR2GRAY);
	GaussianBlur(hChannel, hChannel, Size(3, 3), 0, 0);
    Mat copyHChannel;
	int flags = connectivity + (255 << 8) + FLOODFILL_FIXED_RANGE;
	Rect ccomp;
	int area;
    int counter = 0;
    bool worked = false;
    Point copySeedPoint;
    while(counter < 4){
        hChannel.copyTo(copyHChannel);
        int valx =  seedPoint.x + 2*counter;
        int valy = seedPoint.y - 5*counter;
        copySeedPoint.x = valx > src.size().width ? src.size().width : valx;
        copySeedPoint.y = valy < 0 ? 0 : valy;
	    area = floodFill(copyHChannel, copySeedPoint, Scalar(255), &ccomp, Scalar(20),
		Scalar(20), flags);
        if(area > ((float)src.cols * (float)src.rows * 0.15)) {
            worked = true;
            threshold(copyHChannel, copyHChannel, 254.0, 255.0, THRESH_BINARY);
            dilate(copyHChannel, copyHChannel, Mat());
            break;
        }
        counter++;
    }
	if(worked == false){
        Mat empty;
       return empty;
    }
	return copyHChannel;

}


vector<Point2f> OpenCVMain::getRectanglePoints(Mat mask) {
	Mat canny;
	Point seedPoint;
	seedPoint.x = (float)mask.size().width / 2.0;
	seedPoint.y = (float)mask.size().height / 2.0;
	Canny(mask, canny, 30, 30 * 3);
	vector<Vec2f> lines;
	HoughLines(canny, lines, 1, CV_PI / 180, 100);
	vector<Vec2f> linesHor;
	vector<Vec2f> linesVer;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float theta = lines[i][1];
		if ((theta > 0 && theta < CV_PI / 4) || (theta > CV_PI * 3 / 4 && theta < CV_PI * 5 / 4) || (theta > CV_PI * 7 / 4))
			linesVer.push_back(lines[i]);
		else
			linesHor.push_back(lines[i]);


	}

	double verRightLeve = 0, verRightVal = -1;
	double verLeftLeve = 0, verLeftVal = -1;
	for (int i = 0; i < linesVer.size(); i++) {
		vector<Point> points = OpenCVOperations::getPointsFromLine(linesVer[i]);
		double dis;
		if (points.at(0).y < points.at(1).y)
			dis = OpenCVOperations::distance_to_Line(points.at(1), points.at(0), seedPoint);
		else
			dis = OpenCVOperations::distance_to_Line(points.at(0), points.at(1), seedPoint);

		if (dis > 0 && dis > verRightLeve) {
			verRightLeve = dis;
			verRightVal = i;
		}
		if (dis < 0 && dis < verLeftLeve) {
			verLeftLeve = dis;
			verLeftVal = i;
		}
	}

	double horTopLeve = 0, horTopVal = -1;
	double horBottomLeve = 0, horBottomVal = -1;
	for (int i = 0; i < linesHor.size(); i++) {
		vector<Point> points = OpenCVOperations::getPointsFromLine(linesHor[i]);
		double dis;
		if (points.at(0).x < points.at(1).x) {
			dis = OpenCVOperations::distance_to_Line(points.at(1), points.at(0), seedPoint);
		}
		else {
			dis = OpenCVOperations::distance_to_Line(points.at(0), points.at(1), seedPoint);
		}

		if (dis > 0 && dis > horTopLeve) {
			horTopLeve = dis;
			horTopVal = i;
		}
		if (dis < 0 && dis < horBottomLeve) {
			horBottomLeve = dis;
			horBottomVal = i;
		}
	}
    vector<Point2f> returnPoints;
	vector<Point> pointsLeft;
	vector<Point> pointsTop;
	vector<Point> pointsRight;
	vector<Point> pointsBottom;
	if (verLeftVal != -1) {
		pointsLeft = OpenCVOperations::getPointsFromLine(linesVer[verLeftVal]);
		line(mask, pointsLeft.at(0), pointsLeft.at(1), Scalar(128));
	}
	if(horTopVal != -1) {
		pointsTop = OpenCVOperations::getPointsFromLine(linesHor[horTopVal]);
		line(mask, pointsTop.at(0), pointsTop.at(1), Scalar(128));
	}
	if (verRightVal != -1) {
		pointsRight = OpenCVOperations::getPointsFromLine(linesVer[verRightVal]);
		line(mask, pointsRight.at(0), pointsRight.at(1), Scalar(128));
	}

	if (horBottomVal != -1) {
		pointsBottom = OpenCVOperations::getPointsFromLine(linesHor[horBottomVal]);
		line(mask, pointsBottom.at(0), pointsBottom.at(1), Scalar(128));
	}

	
	
	
	if (verLeftVal == -1) {
		if (horTopVal != -1 && horBottomVal != -1) {
			pointsLeft.push_back(OpenCVOperations::pointFromLineAtX(pointsTop, 0));
			pointsLeft.push_back(OpenCVOperations::pointFromLineAtX(pointsBottom, 0));
		}
		else {
			pointsLeft.push_back(Point(PADDING, PADDING));
			pointsLeft.push_back(Point(PADDING, mask.size().height - PADDING));
		}
	}

	if (horTopVal == -1) {
		if (verLeftVal != -1 && verRightVal != -1) {
			pointsTop.push_back(OpenCVOperations::pointFromLineAtY(pointsLeft, 0));
			pointsTop.push_back(OpenCVOperations::pointFromLineAtY(pointsRight, 0));
		}
		else {
			pointsTop.push_back(Point(PADDING, PADDING));
			pointsTop.push_back(Point(mask.size().width - PADDING, PADDING));
		}
	}


	if (verRightVal == -1) {
		if (horTopVal != -1 && horBottomVal != -1) {
			pointsRight.push_back(OpenCVOperations::pointFromLineAtX(pointsTop, mask.size().width));
			pointsRight.push_back(OpenCVOperations::pointFromLineAtX(pointsBottom, mask.size().width));
		}
		else {
			pointsRight.push_back(Point(mask.size().width - PADDING, PADDING));
			pointsRight.push_back(Point(mask.size().width - PADDING, mask.size().height - PADDING));
		}
	}

	if (horBottomVal == -1) {
		if (verLeftVal != -1 && verRightVal != -1) {
			pointsBottom.push_back(OpenCVOperations::pointFromLineAtY(pointsLeft, mask.size().height));
			pointsBottom.push_back(OpenCVOperations::pointFromLineAtY(pointsRight, mask.size().height));
		}
		else {
			pointsBottom.push_back(Point(PADDING, mask.size().height - PADDING));
			pointsBottom.push_back(Point(mask.size().width - PADDING, mask.size().height - PADDING));
		}
	}

	imshow("ImageWithLines", mask);
	waitKey(1);
	vector<bool> intersect;
	intersect.push_back(OpenCVOperations::getIntersectionPoint(pointsLeft, pointsTop, &returnPoints));
	intersect.push_back(OpenCVOperations::getIntersectionPoint(pointsTop, pointsRight, &returnPoints));
	intersect.push_back(OpenCVOperations::getIntersectionPoint(pointsRight, pointsBottom, &returnPoints));
	intersect.push_back(OpenCVOperations::getIntersectionPoint(pointsBottom, pointsLeft, &returnPoints));

    bool error = false;
    for (int i = 0; i < intersect.size(); i++) {
        if (intersect.at(i) == true) {
            if (!OpenCVOperations::checkPointInImage(returnPoints.at(i), mask)) {
                if (i == 0)
                    returnPoints.at(i) = OpenCVOperations::pointFromLineAtX(pointsTop, 0);
                if(i == 1)
                    returnPoints.at(i) = OpenCVOperations::pointFromLineAtX(pointsTop, mask.size().width);
                if (i == 2)
                    returnPoints.at(i) = OpenCVOperations::pointFromLineAtX(pointsBottom, mask.size().width);
                if (i == 3)
                    returnPoints.at(i) = OpenCVOperations::pointFromLineAtX(pointsBottom, 0);
            }
            //circle(cdst, returnPoints.at(i), 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
        }
        else {
            error = true;
        }
    }
    if (error) {
        returnPoints.clear();
    }
    return returnPoints;
}


