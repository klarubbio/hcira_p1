/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/


#include <SFML/Graphics.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include "TemplateMap.h"
#include "fillTemplateMap.h"
#include "recognizer.h"
#include "parseXML.h"
using namespace std;


int main() {
    using boost::property_tree::ptree;  
    
    vector<TemplateMap> rawUserData;
    parseXML(rawUserData);
    
    //for each gesture type in map
    TemplateMap processed;
    for (auto itr = rawUserData[0].templates.begin(); itr != rawUserData[0].templates.end(); itr++) {
        for (unsigned int j = 0; j < itr->second.size(); j++) {
            //cout << "preprocessing" << endl;
            vector<Point> resampled;
            resample(itr->second[j], 64, resampled);
            //Rotation function calls
            vector<Point> rotated;
            rotateToZero(resampled, 64, rotated);
            //Scaling & translation function calls
            vector<Point> scaled;
            scaled = ScaleTo(rotated, 400);
            vector<Point> translated;
            translated = TranslateTo(scaled, Point(200, 200));
            //Add the template to the templatemap for the current user
            processed.addTemplate(itr->first, translated);
        }
    }

    string test = "arrow";
    vector<Point>& points = processed.templates[test].at(0);
    pair<string, double> result = Recognize(points, processed);

    cout << "Tested with: " << test << endl;
    cout << "Got:         " << result.first << endl;
    return 0;
}