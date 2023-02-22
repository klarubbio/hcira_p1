#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "TemplateMap.h"


using boost::property_tree::ptree;
using namespace std;

void parseXML(vector<TemplateMap>& userTemplates);