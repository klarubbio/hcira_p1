#include "recognizer.h"
#include <iostream>
using namespace std;

//Resample a points path into n evenly spaced points
void resample(vector<Point>& points,  int n, vector<Point>& resampled) {
	double resampledDistance = pathLength(points) / (n - 1); //how far apart an even spacing would be
	//cout << resampledDistance << endl;
	double distFromResampled = 0; //the distance from the last resampled point or first point (will not be a straight line but rather the sum of distance from point to point)
	resampled.push_back(points[0]);
	for (int i = 1; i < points.size(); i++) {
		double distFromLast = points[i].distance(points[i-1]); //current distance between two points
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