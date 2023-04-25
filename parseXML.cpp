/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "TemplateMap.h"
#include "parseXML.h"
using namespace std;

const ptree& empty_ptree() {
	static ptree t;
	return t;
}

void parseOneXML(vector<Point>& points, string path) {
	ptree pt;
	read_xml(path, pt);
	const ptree& formats = pt.get_child("Gesture", empty_ptree());

	BOOST_FOREACH(const ptree::value_type & child, formats) {
		const ptree& attributes = child.second.get_child("<xmlattr>", empty_ptree());
		int i = 0;
		Point p = Point(0, 0);
		BOOST_FOREACH(const ptree::value_type & value_type, attributes) {
			//cout << value_type.second.data() << endl; 
			if (i == 1) {
				// x value
				p.x = stod(value_type.second.data());
			}
			else if (i == 2) {
				// y value
				p.y = stod(value_type.second.data());
			}

			i++;
		}
		points.push_back(p);
	}
}

void parseXML(vector<TemplateMap>& userTemplates) {
	vector<string> users = { "s02", "s03", "s04", "s05", "s06", "s07", "s08", "s09", "s10", "s11" };
	vector<string> speeds = { "slow", "medium", "fast" };
	vector<string> gestures = { "arrow", "caret", "check", "circle", "delete", "left_curly_brace", "left_square_bracket", "pigtail", "rectangle", "right_curly_brace", "right_square_bracket", "star", "triangle", "v", "x", "zigzag" };

	//vector<TemplateMap> userTemplates;

	/* 
	* testing to see where the file path should start (start at xml folder, not c drive)
	std::ifstream myFile;
	myFile.open("xml/xml_logs/s02/slow/arrow01.xml");
	if (myFile.is_open()) {
		cout << "worked";
	}

	ptree pt;
	read_xml("xml/xml_logs/s02/slow/arrow01.xml", pt);*/

	// for loop for users
  // ERROR in file directory, need help here
	string currentDirectory = "xml_eyes/"; // current directory where this folder is stored
	for (int i = 2; i < 3; i++) {
		string userDirectory = currentDirectory;
		if (i < 10) {
			userDirectory = userDirectory + "s0" + std::to_string(i);
		}
		else {
			userDirectory = userDirectory + "s" + std::to_string(i);
		}
		string speed = speeds[0]; // slow, can choose whataver we like
		string templateDirectory = userDirectory;
		//cout << templateDirectory << endl;

		TemplateMap currentUserMap;

		// 16 gestures
		for (int j = 0; j < gestures.size(); j++) {
			string gestureDirectory = templateDirectory + "/" + gestures[j];
			vector<Point> _template;
			for (int k = 1; k < 11; k++) {
				if (k < 10) {
					gestureDirectory = gestureDirectory + "0" + std::to_string(k) + ".xml";
				}
				else {
					gestureDirectory = gestureDirectory + std::to_string(k) + ".xml";
				}
				//cout << gestureDirectory << endl;
				vector<Point> curr_template = _template;


				// parse xml
				// each user should have their own template map --> vector<TemplateMap> userTemplates
				// each xml file will have <Gesture Name="name01" NumPts="84">
				// each gesture has NumPts <Point X="x-val" Y="y-val" T="?" />

				ptree pt;
				read_xml(gestureDirectory, pt);
				const ptree& formats = pt.get_child("Gesture", empty_ptree());

				BOOST_FOREACH(const ptree::value_type & child, formats) {
					const ptree& attributes = child.second.get_child("<xmlattr>", empty_ptree());
					int i = 0;
					Point p = Point(0,0);
					BOOST_FOREACH(const ptree::value_type & value_type, attributes) {
						//cout << value_type.second.data() << endl; 
						if (i == 1) {
							// x value
							p.x = stod(value_type.second.data());
						}
						else if (i == 2) {
							// y value
							p.y = stod(value_type.second.data());
						}
						
						i++;
					}
					curr_template.push_back(p);
				}
				//double a = child.second.get<double>("<xmlattr.X");
				//double b = child.second.get<double>("<xmlattr.Y");
				//Point p = Point(a, b);
				
				currentUserMap.addTemplate(gestures[j], curr_template);
				gestureDirectory = templateDirectory + "/" + gestures[j];
			}
			
		}
		//currentUserMap.printTemplateMap();

		userTemplates.push_back(currentUserMap);
	}
};
