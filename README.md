# HCIRA_P1
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson

CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida

Spring 2023

Use for SFML setup: https://www.sfml-dev.org/tutorials/2.5/start-vc.php

Use for Boost XML setup:
Download: https://www.boost.org/users/download/
Install: 
- go into project properties
- go into C/C++ > General > Additional Include Dependencies
- add a the file directory for this package: downloads (or wherever boost was extracted)\boost_1_81_0\boost_81_0
- go into Linker > General > Additional Library Dependencies
- add a the file directory for this package: downloads (or wherever boost was extracted)\boost_1_81_0\boost_81_0\boost

Working on Visual Studio 2019 with SFML for VS C++ 15
## Part 4
### Write gesture files
### Prompt for specific samples
### Recruit 6 people
### Submit full dataset

## Part 3
### Read in Dataset
main.cpp - Lines 35-37 & parseXML() function - This function parses the XML logs folder using the boost library to fill in a vector of TemplateMap objects. It navigates between the file directory and subdirectories in order to get the data from all 10 users for every gesture. Each user has their own TemplateMap, and their XML data is stored within the TemplateMap.
### Connect to Recognizer
main.cpp - Lines 36-59 - The data structure containing the raw user data is looped through. A new data structure (vector<TemplateMap> preprocessedUserData) is created to hold the preprocessed data which will be used for recognition while maintaining raw data points. The functions from the recognizer in recognizer.cpp (resample, rotateToZero, scaleTo, translateTo) are called on each point. Each user's data is represented as a TemplateMap within the vector, which contains their respective samples for each gesture type. 
### Loop over Dataset
main.cpp - Lines 64-204 - For each user, the code will choose 1 to 9 random examples per gesture from the user and iterate 10 times through each gesture, and choose candidate gestures. The candidate gestures are run through the recognizer with the set of all random examles of each gesture for each user. For each run of the recognizer, a log is saved in a vector of vectors of strings.
### Output the Result
main.cpp Lines 114-217 - Each log entry is a vector of strings, each value in the vector will be a column attribute in the CSV. After a log entry has been completed for each call to the recognizer, it gets added to our vector of vectors of strings. This object is looped through at the end of the program and each log entry is written with its values comma seperated and a new line at the end of every log entry.


## Part 2: Online/Live Recognition
### Store the Points
main.cpp - Lines 78-108 - Points are recorded as the user moves their mouse while pressing a mouse button. These points are stored each time that the user releases their mouse button in a vector of points called shape. 
### Store some Templates
main.cpp - Lines 20-24, fillTemplateMap.cpp - fillTemplateMap(TemplateMap &templateMap, TemplateMap& preprocessed) function at Line 11 - Points for each template were copied from the Javascript online (See source 5 below).
### Implement $1 Algorithm
- Resampling
  - main.cpp - Line 113, recognizer.cpp - resample(vector<Point>& points,  int n, vector<Point>& resampled) function at Line 15
  - Since the user draws a shape at a variable speed, different numbers of points are created each time. In order to adequately compare the user's drawing to our stored templates, we must resample the drawing into N different points. In this case, N = 64.
- Rotation
  - main.cpp - Line 117, recognizer.cpp - rotateToZero(vector<Point>& points, int n, vector<Point>& rotated) function at line 56
  - Each drawing will always have a slight rotation to it due to different user input. Once again, in order to compare their drawing to the templates properly the drawing must be rotated properly. Each Drawing is rotated to 0.
- Scaling and Translation
  - main.cpp - Lines 119-122, recognizer.cpp - ScaleTo(vector<Point> points, double size) function at line 112 and TranslateTo(vector<Point> points, Point point) function at line 138
  - Since each drawing could be drawn at different sizes, it must be scaled down to a size that can be compared to the templates. These drawings also occur at different points on the canvas, so they must also be translated. This happens by creating a box that captures the x-max, x-min, y-max, and y-min values of the set of coordinates, then scaling them appropriately by using the shapes centroid. The translation shifts the drawing to a point on the canvas that is constant for all templates.
- Matching Process
  - main.cpp - Line 124, recognizer.cpp - Recognize(vector<Point>& points, TemplateMap templates) function at line 150
  - After the previous set of steps, the drawing is now able to be compared appropriately. The matching process works by going through each template and comparing the differences in the distances of the sets of points. The template with the greatest score, or least difference between points, is returned as the matching value.
### Output the Result
  - main.cpp - Line 124, 151
  - Results from the matching process are output to the lower left hand corner and not updated until a new drawing is made.

## Part 1: Drawing on a Canvas
### Set up a Development Environment
This C++ project is being run in Visual Studio using the SFML library. See the above links for more information about our development environment. The main.cpp file has dependencies in TemplateMap.cpp and Point.cpp that we have started for later use, although they are not directly relevant to this project component.
### Instantiating a Canvas
main.cpp - Lines 37-39 (main) - Instantiate a sf::RenderWindow object, which will act as our canvas
main.cpp - Line 76  (main)- Open the canvas window
### Listening for Mouse or Touch Events
main.cpp - Lines 80-123  (main)- Listens for events and handles them accordingly. Each new mouse press (sf::Event::MouseButtonPressed) is added to a list of vertices (sf::VertexArray vertices) so long as it was not recorded on the clear button. If the mouse moves before the button is released, more vertices are added (sf::Event::MouseMoved). When the mouse button is released (sf::Event::MouseButtonReleased), drawing stops. 
### Clearing the Canvas
main.cpp - Lines 46-68  (main)- Loads and sets the position of the clear button.
main.cpp - Lines 107-121  (main)- Clears the list of vertices. When vertices is redrawn, there will be no points, yielding an empty canvas. 

## Sources
1. Using sf::RenderWindow to create GUI: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1RenderWindow.php
2. Using sf::VertexArray for drawing: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
3. Using sf::Events for registering mouse clicks: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Event.php
4. Helpful forum for drawing approach: https://stackoverflow.com/questions/53085055/using-sfml-to-draw-stuff
5. Using premade templates: http://depts.washington.edu/acelab/proj/dollar/index.html
6. Algorithm modeled after the pseudocode here: https://dl.acm.org/doi/10.1145/1294211.1294238
7. Library and documentation for XML parsing: https://www.boost.org/users/download/

