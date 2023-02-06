#pragma once

class Point {
public:
	Point(double xCoord, double yCoord);
	double distance(Point other);
	double x;
	double y;
};