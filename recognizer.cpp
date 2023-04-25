/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/

#include "recognizer.h"
#include <numeric>
#include <limits>
#include <algorithm>
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
		//cout << "Current distance from last point: " << distFromResampled << endl;
		//cout << "Target distance between points: " << resampledDistance << endl;
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

Rectangle BoundingBox(vector<Point> points) {
	double xMin = numeric_limits<double>::infinity();
	double xMax = numeric_limits<double>::infinity() * -1;
	double yMin = numeric_limits<double>::infinity();
	double yMax = numeric_limits<double>::infinity() * -1;

	for (int i = 0; i < points.size(); i++) {
		xMin = min(xMin, points[i].x);
		xMax = max(xMax, points[i].x);
		yMin = min(yMin, points[i].y);
		yMax = max(yMax, points[i].y);
	}

	return Rectangle(xMin, yMin, xMax-xMin, yMax-yMin);
}

vector<Point> ScaleTo(vector<Point> points, double size) {
	// create Rectangle class
	// create BoundingBox function
	Rectangle box = BoundingBox(points);
	vector<Point> newPoints;
	for (int i = 0; i < points.size(); i++) {
		double newX = points[i].x * (size / box.width);
		double newY = points[i].y * (size / box.height);
		newPoints.push_back(Point(newX, newY));
	}

	return newPoints;
}

Point Centroid(vector<Point> points) {
	double x = 0;
	double y = 0;
	for (int i = 0; i < points.size(); i++) {
		x += points[i].x;
		y += points[i].y;
	}
	x /= points.size();
	y /= points.size();
	return Point(x, y);
}

vector<Point> TranslateTo(vector<Point> points, Point point) {
	Point centroid = Centroid(points);
	vector<Point> newPoints;
	for (int i = 0; i < points.size(); i++) {
		double newX = points[i].x + point.x - centroid.x;
		double newY = points[i].y + point.y - centroid.y;
		newPoints.push_back(Point(newX, newY));
	}

	return newPoints;
}

pair<string, double> Recognize(vector<Point>& points, TemplateMap templates) {
	double best = numeric_limits<double>::infinity();
	string bestName = "";

	//set constants
	double theta = 45.0;
	double thetaDelta = 2.0;

	//Look through all best distances and select lowest
	for (auto itr = templates.templates.begin(); itr != templates.templates.end(); itr++) {
		for (int i = 0; i < itr->second.size(); i++) {
			double distance = DistanceAtBestAngle(points, itr->second[i], -theta, theta, thetaDelta);

			if (distance < best) {
				best = distance;
				bestName = itr->first;
			}
		}
	}

	double score = 1 - (best / (0.5 * sqrt(pow(1500, 2) + pow(1500, 2))));
	//return best score and corresponding template name
	return make_pair(bestName, score);
}

double DistanceAtBestAngle(vector<Point>& points, vector<Point>& compare, double thetaA, double thetaB, double thetaDelta) {
	//math to find the best angle theta (lowest distance)
	double phi = 0.5 * (-1 + sqrt(5));
	double x1 = (phi * thetaA) + (1 - phi) * thetaB;
	double f1 = DistanceAtAngle(points, compare, x1);
	double x2 = (1 - phi) * thetaA + (phi * thetaB);
	double f2 = DistanceAtAngle(points, compare, x2);
	//keep calculating until the distance between angles is acceptable
	while (abs(thetaB - thetaA) > thetaDelta) {
		if (f1 < f2) {
			thetaB = x2;
			x2 = x1;
			f2 = f1;
			x1 = (phi * thetaA) + (1 - phi) * thetaB;
			f1 = DistanceAtAngle(points, compare, x1);
		}
		else {
			thetaA = x1;
			x1 = x2;
			f1 = f2;
			x2 = (1 - phi) * thetaA + (phi * thetaB);
			f2 = DistanceAtAngle(points, compare, x2);
		}
	}
	return min(f1, f2);
}

//helper function to find the distance between points at an angle theta
double DistanceAtAngle(vector<Point>& points, vector<Point>& compare, double theta) {
	vector<Point> rotated;
	rotateBy(points, points.size(), theta, rotated);
	double distance = PathDistance(rotated, compare);
	return distance;
}

//helper function to find path distance between a vector of points (point to point to point rather than the linear distance between furthest points)
double PathDistance(vector<Point>& points, vector<Point>& compare) {
	double distance = 0.0;
	for (int i = 0; i < points.size(); i++) {
		distance += points[i].distance(compare[i]);
	}
	return (distance / points.size());
}