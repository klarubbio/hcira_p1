#pragma once

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