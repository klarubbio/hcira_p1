/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/


#include <SFML/Graphics.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include "TemplateMap.h"
#include "fillTemplateMap.h"
#include "recognizer.h"
using namespace std;


int main()
{
    /*
    // boost sanity check (test this after installing boost library by typing "1 2 3")
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " ");*/
    
    vector<TemplateMap> templates;
    //FILL FROM XML HERE

    // Preprocess points from XML file and save to preprocessedTemplates data structure
    vector<TemplateMap> preprocessedTemplates; //list in which each item is a different user's template map
    for (int i = 0; i < templates.size(); i++) {
        TemplateMap preprocessed;
        for (auto itr = templates[i].templates.begin(); itr != templates[i].templates.begin(); itr++) {
            for (int j = 0; j < itr->second.size(); j++) {
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
                preprocessed.addTemplate(itr->first, translated);
            }
        }
        //add to vector containing all users' samples
        preprocessedTemplates.push_back(preprocessed);
    }

    return 0;
}

    /*REMOVE COMMENT TO USE GUI
    vector<Point> shape;
	fillTemplateMap(templateMap, preprocessedTemplates);
    //templateMap.printTemplateMap();
    
    
    int width = 400;
    int height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");
    sf::Texture clearBtn;
    sf::Texture clearBtnPressed;
    sf::Sprite clearBtnSprite;
    sf::Font outputFont;
    sf::Text outputText;

    window.setFramerateLimit(1000);

    // load clear button states (pressed and unpressed)
    if (!clearBtn.loadFromFile("clear.png")) {
        std::cout << "failed to load clear.png";
    }

    if (!clearBtnPressed.loadFromFile("clear_pressed.png")) {
        std::cout << "failed to load clear_pressed.png";
    }

    if (!outputFont.loadFromFile("Roboto-Black.ttf")) {
        std::cout << "failed to load Roboto-Black.ttf";
    }

    clearBtnSprite.setPosition(300, 350);
    clearBtnSprite.setTexture(clearBtn);

    outputText.setPosition(0, 375);
    outputText.setFont(outputFont);
    outputText.setCharacterSize(24);
    outputText.setFillColor(sf::Color::Blue);

    //tracks actual points drawn
    //sf::VertexArray vertices(sf::LineStrip);
    vector<sf::Vertex> vertices;
    vector<sf::Vertex> resampledVisualization;

    sf::Vector2f last;
    bool drawing = false;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //visualize first point of contact
            else if (event.type == sf::Event::MouseButtonPressed) {
                // if pressed clear btn, do not draw
                if (event.mouseButton.x >= 300 && event.mouseButton.y >= 350) {
                    clearBtnSprite.setTexture(clearBtnPressed);
                }
                else {
                    //start new stroke
                    
                    vertices.clear();
                    drawing = true;
                    last = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    vertices.push_back(sf::Vertex(last, sf::Color(0, 0, 0)));
                    break;
                }
            }
            //connect additional points of contact
            else if (event.type == sf::Event::MouseMoved && drawing) {
                const sf::Vector2f next(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                vertices.push_back(sf::Vertex(next, sf::Color(0, 0, 0)));
                last = next;
                break;
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (drawing) {
                    drawing = false;
                    Point origin = Point(0, 0);
                    for (int i = 0; i < vertices.size(); i++) {
                        shape.push_back(Point(vertices[i].position.x, vertices[i].position.y));
                        //cout << "Test: Distance from origin: " << shape.at(i).distance(origin) << endl;
                        //cout << vertices[i].position.x << " " << vertices[i].position.y << endl;
                    }

                    //Resampling function calls
                    vector<Point> resampled;
                    cout << "Original points: " << shape.size() << endl;
                    resample(shape, 64, resampled);
                    cout << "Resampled Points: " << resampled.size() << endl;
                    //Rotation function calls
                    vector<Point> rotated;
                    rotateToZero(resampled, resampled.size(), rotated);
                    //Scaling & translation function calls
                    vector<Point> scaled;
                    scaled = ScaleTo(rotated, width);
                    vector<Point> translated;
                    translated = TranslateTo(scaled, Point(width/2.0,height/2.0));
                    //Recognize
                    outputText.setString(" " + Recognize(translated, preprocessedTemplates).first);


                    //visualize resamped and rotated and translated points for fun, comment out when not debugging
                    /*
                    for (int i = 0; i < rotated.size(); i++) {
                        resampledVisualization.push_back(sf::Vertex(sf::Vector2f(translated[i].x, translated[i].y), sf::Color(0, 0, 0)));
                        //cout << translated[i].x << " " << translated[i].y << endl;
                    }*/
                    // REMOVE COMMENT TO USE GUI break;
                //}
                // if mouse btn released outside of clear btn, do not clear
                /* REMOVE COMMENT TO USE GUI
                else if (clearBtnSprite.getTexture() == &clearBtnPressed && (event.mouseButton.x < 300 || event.mouseButton.y < 350)) {
                    clearBtnSprite.setTexture(clearBtn);;
                }
                // clear
                else if (clearBtnSprite.getTexture() == &clearBtnPressed && event.mouseButton.x >= 300 && event.mouseButton.y >= 350) {
                    clearBtnSprite.setTexture(clearBtn);
                    vertices.clear();
                    resampledVisualization.clear();
                }
            }
            
        }

        window.clear(sf::Color(255,255,255));
        window.draw(clearBtnSprite);
        window.draw(outputText);
        //draw only actual points recorded and lines in between
        if (!vertices.empty() && resampledVisualization.empty()) {
            resampledVisualization.clear();
            shape.clear();
            window.draw(&vertices[0], vertices.size() - 1, sf::LineStrip);
        }
        /*
        //comment out when not debugging
        if (!resampledVisualization.empty()) {
            window.draw(&resampledVisualization[0], resampledVisualization.size() - 1, sf::Points);
        }*/ /* REMOVE COMMENT TO USE GUI
        window.display();

    }
    
                return 0;

    
}*/
