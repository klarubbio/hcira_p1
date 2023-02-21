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


void parseXML() {
	vector<string> users = { "s02", "s03", "s04", "s05", "s06", "s07", "s08", "s09", "s10", "s11" };
	vector<string> speeds = { "slow", "medium", "fast" };
	vector<string> gestures = { "arrow", "caret", "check", "circle", "delete_mark", "left_curly_brace", "left_sq_bracket", "pigtail", "question_mark", "rectangle", "right_curly_brace", "right_sq_bracket", "star", "triangle", "v", "x" };

	vector<TemplateMap> userTemplates;

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
	string currentDirectory = "xml/xml_logs/"; // current directory where this folder is stored
	for (int i = 2; i < 12; i++) {
		string userDirectory = currentDirectory;
		if (i < 10) {
			userDirectory = userDirectory + "s0" + std::to_string(i);
		}
		else {
			userDirectory = userDirectory + "s" + std::to_string(i);
		}
		string speed = speeds[0]; // slow, can choose whataver we like
		string templateDirectory = userDirectory + "/" + speed;
		cout << templateDirectory << endl;

		TemplateMap currentUserMap;

		// 16 gestures
		for (int j = 0; j < gestures.size(); j++) {
			userDirectory = templateDirectory + "/" + gestures[j];
			vector<Point> _template;
			for (int k = 1; k < 11; k++) {
				if (k < 10) {
					userDirectory = userDirectory + "0" + std::to_string(k) + ".xml";
				}
				else {
					userDirectory = userDirectory + std::to_string(k) + ".xml";
				}
				cout << userDirectory << endl;
				vector<Point> curr_template = _template;


				// parse xml
				// each user should have their own template map --> vector<TemplateMap> userTemplates
				// each xml file will have <Gesture Name="name01" NumPts="84">
				// each gesture has NumPts <Point X="x-val" Y="y-val" T="?" />

				ptree pt;
				read_xml(userDirectory, pt);
				const ptree& formats = pt.get_child("Gesture", empty_ptree());

				BOOST_FOREACH(const ptree::value_type & child, formats) {
					const ptree& attributes = child.second.get_child("<xmlattr>", empty_ptree());
					int i = 0;
					Point p;
					BOOST_FOREACH(const ptree::value_type & value_type, attributes) {
						cout << value_type.second.data() << endl; //ptree bad path thrown here
						if (i == 0) {
							// x value
							p.x = value_type.second.data();
						}
						else if (i == 1) {
							// y value
							p.y = value_type.second.data();
						}
						
						i++;
					}
				}
				//double a = child.second.get<double>("<xmlattr.X");
				//double b = child.second.get<double>("<xmlattr.Y");
				//Point p = Point(a, b);
				//curr_template.push_back(p);

				currentUserMap.addTemplate(gestures[i], curr_template);
			}
			userDirectory = templateDirectory;
		}
		currentUserMap.printTemplateMap();

		userTemplates.push_back(currentUserMap);
	}
};
