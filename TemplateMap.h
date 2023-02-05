#pragma once

#include "Point.h"
#include <Map>
#include <Vector>
#include <string>
#include <iostream>

using namespace std;

class TemplateMap {
private:
	map<string, vector<vector<Point>>> templates;
public:
	TemplateMap();
	void addTemplate(string templateName, vector<Point> newTemplate);
	void clearTemplates(string templateName);
	void printTemplateMap();
};