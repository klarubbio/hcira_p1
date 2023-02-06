#include "Point.h"
#include <math.h>
#include <iostream>
using namespace std;


Point::Point(double xCoord, double yCoord) {
	x = xCoord;
	y = yCoord;
}

double Point::distance(Point other) {
	return sqrt((pow((x - other.x), 2.0)) + pow((y - other.y), 2.0));
}