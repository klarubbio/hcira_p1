#pragma once
/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/

class Point {
public:
	Point(double xCoord, double yCoord);
	double distance(Point other);
	double x;
	double y;
};