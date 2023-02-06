#include "recognizer.h"
#include <numeric>
#include <iostream>
using namespace std;

//Resample a points path into n evenly spaced points
void resample(vector<Point>& points,  int n, vector<Point>& resampled) {
	double resampledDistance = pathLength(points) / (n - 1); //how far apart an even spacing would be
	//cout << resampledDistance << endl;
	double distFromResampled = 0; //the distance from the last resampled point or first point (will not be a straight line but rather the sum of distance from point to point)
	resampled.push_back(points[0]);
	double distFromLast = 0;
	for (int i = 1; i < points.size(); i++) {
		distFromLast = points[i].distance(points[i-1]); //current distance between two points
		//cout << "distance from last point: " << distFromLast << endl;
		if ((distFromResampled + distFromLast) >= resampledDistance) {
			//cout << "Point got far enough away" << endl;
			//does the Point type need to take doubles? I think yes but let's see
			double newX = points[i - 1].x + (((resampledDistance - distFromResampled)/distFromLast) * (points[i].x - points[i-1].x));
			double newY = points[i - 1].y + (((resampledDistance - distFromResampled) / distFromLast) * (points[i].y - points[i - 1].y));
			resampled.push_back(Point(newX, newY));
			points.insert(points.begin() + i, Point(newX, newY));
			distFromResampled = 0;
		}
		else {
			//cout << "Keep looking for further point" << endl;
			distFromResampled += distFromLast;
		}
	}
	
	if (resampled.size() < n) {
		cout << "Current distance from last point: " << distFromResampled << endl;
		cout << "Target distance between points: " << resampledDistance << endl;
		resampled.push_back(points[points.size() - 1]);
	}
}

//Calculate entire path length for resampling
float pathLength(vector<Point>& points) {
	double distance = 0;
	for (int i = 1; i < points.size(); i++) {
		distance += points[i].distance(points[i-1]);
	}
	//cout << distance << endl;
	return distance;
}

void rotateToZero(vector<Point>& points, int n, vector<Point>& rotated) {
	// get centroid
	double xAvg = 0;
	double yAvg = 0;

	for (int i = 0; i < n; i++) {
		xAvg += points[i].x;
		yAvg += points[i].y;
	}

	xAvg = xAvg / n;
	yAvg = yAvg / n;

	// get theta and rotate
	double theta = atan2(yAvg - points[0].y, xAvg - points[0].x);
	rotateBy(points, n, -theta, rotated);
}

void rotateBy(vector<Point>& points, int n, double theta, vector<Point>& rotated) {
	// get centroid
	double xAvg = 0;
	double yAvg = 0;

	for (int i = 0; i < n; i++) {
		xAvg += points[i].x;
		yAvg += points[i].y;
	}

	xAvg = xAvg / n;
	yAvg = yAvg / n;

	// rotate
	for (int i = 0; i < n; i++) {
		Point newPoint(0, 0);
		newPoint.x = ((points[i].x - xAvg) * cos(theta)) - ((points[i].y - yAvg) * sin(theta)) + xAvg;
		newPoint.y = ((points[i].x - xAvg) * sin(theta)) + ((points[i].y - yAvg) * cos(theta)) + yAvg;
		rotated.push_back(newPoint);
	}
}