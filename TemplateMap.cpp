#include "Point.h"
#include "TemplateMap.h"
#include <Map>
#include <Vector>
#include <string>
#include <iostream>

using namespace std;

TemplateMap::TemplateMap() {

}

void TemplateMap::addTemplate(string templateName, vector<Point> newTemplate) {
	templates[templateName].push_back(newTemplate);
}

void TemplateMap::clearTemplates(string templateName) {
	templates[templateName].clear();
}

void TemplateMap::printTemplateMap() {
	for (map<string, vector<vector<Point>>>::iterator it = templates.begin(); it != templates.end(); ++it) {
		cout << it->first << endl; // name of template
		// loop through vector of vectors
		for (vector<Point> templatesVector : it->second) {
			cout << "  template" << endl;
			// loop through Points and display as (x,y)
			for (Point point : templatesVector) {
				cout << "    (" << point.x << ", " << point.y << ")" << endl;
			}
		}
	}
}


