/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/

#include "Point.h"
#include <math.h>
#include <iostream>
using namespace std;


Point::Point(double xCoord, double yCoord) {
	x = xCoord;
	y = yCoord;
}

double Point::distance(Point other) {
	//use round to help with loss of precision
	return sqrt((pow((x - other.x), 2.0)) + pow((y - other.y), 2.0));
}