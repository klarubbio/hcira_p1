#pragma once
#include "Point.h"
#include "Rectangle.h"
#include <vector>
using namespace std;

//Resample a points path into n evenly spaced points
void resample(vector<Point>& points, int n, vector<Point>& resampled);
//Calculate entire path length for resampling
float pathLength(vector<Point>& points); //is this supposed to be float or list return type?

void rotateToZero(vector<Point>& points, int n, vector<Point>& rotated);
void rotateBy(vector<Point>& points, int n, double theta, vector<Point>& rotated);
Rectangle BoundingBox(vector<Point> points); // creates a box within x and y min / max
vector<Point> ScaleTo(vector<Point> points, double size);
