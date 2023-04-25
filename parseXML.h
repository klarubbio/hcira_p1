/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "TemplateMap.h"


using boost::property_tree::ptree;
using namespace std;

void parseXML(vector<TemplateMap>& userTemplates);
void parseOneXML(vector<Point>& points, string path);
