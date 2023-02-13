#pragma once

/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/

#include "Point.h"
#include <Map>
#include <Vector>
#include <string>
#include <iostream>

using namespace std;

class TemplateMap {
public:
	map<string, vector<vector<Point>>> templates;
	TemplateMap();
	void addTemplate(string templateName, vector<Point> newTemplate);
	void clearTemplates(string templateName);
	void printTemplateMap();
};