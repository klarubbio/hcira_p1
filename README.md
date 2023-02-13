# HCIRA_P1
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023

Use for SFML setup: https://www.sfml-dev.org/tutorials/2.5/start-vc.php

Working on Visual Studio 2019 with SFML for VS C++ 15

## Part 2: Online/Live Recognition
### Store the Points

### Store some Templates

### Implement $1 Algorithm
- Resampling
- Rotation
- Scaling and Translation
- Matching Process
### Output the Result

## Part 1: Drawing on a Canvas (Specific information in this section (line numbers) may not be accurate as of 2/13/23)
### Set up a Development Environment
This C++ project is being run in Visual Studio using the SFML library. See the above links for more information about our development environment. 
### Instantiating a Canvas
main.cpp - Lines 37-39 - Instantiate a sf::RenderWindow object, which will act as our canvas
main.cpp - Line 76 - Open the canvas window
### Listening for Mouse or Touch Events
main.cpp - Lines 80-123 - Listens for events and handles them accordingly. Each new mouse press (sf::Event::MouseButtonPressed) is added to a list of vertices (sf::VertexArray vertices) so long as it was not recorded on the clear button. If the mouse moves before the button is released, more vertices are added (sf::Event::MouseMoved). When the mouse button is released (sf::Event::MouseButtonReleased), drawing stops. 
### Clearing the Canvas
main.cpp - Lines 46-68 - Loads and sets the position of the clear button.
main.cpp - Lines 107-121 - Clears the list of vertices. When vertices is redrawn, there will be no points, yielding an empty canvas. 
