#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "TemplateMap.h"
#include "fillTemplateMap.h"
#include "recognizer.h"
using namespace std;


int main()
{

    TemplateMap templateMap;
    TemplateMap preprocessedTemplates;
    vector<Point> shape;

	fillTemplateMap(templateMap, preprocessedTemplates);
    //templateMap.printTemplateMap();
    
    
    int width = 400;
    int height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");
    sf::Texture clearBtn;
    sf::Texture clearBtnPressed;
    sf::Sprite clearBtnSprite;

    window.setFramerateLimit(1000);

    // load clear button states (pressed and unpressed)
    if (!clearBtn.loadFromFile("clear.png")) {
        std::cout << "failed to load clear.png";
    }

    if (!clearBtnPressed.loadFromFile("clear_pressed.png")) {
        std::cout << "failed to load clear_pressed.png";
    }

    clearBtnSprite.setPosition(300, 350);
    clearBtnSprite.setTexture(clearBtn);

    // load clear button states (pressed and unpressed)
    if (!clearBtn.loadFromFile("clear.png")) {
        std::cout << "failed to load clear.png";
    }

    if (!clearBtnPressed.loadFromFile("clear_pressed.png")) {
        std::cout << "failed to load clear_pressed.png";
    }

    clearBtnSprite.setPosition(300, 350);
    clearBtnSprite.setTexture(clearBtn);

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
                    rotateToZero(resampled, 64, rotated);
                    //Scaling & translation function calls
                    vector<Point> scaled;
                    scaled = ScaleTo(rotated, width);
                    vector<Point> translated;
                    translated = TranslateTo(scaled, Point(width/2.0,height/2.0));
                    //Recognize
                    cout << Recognize(translated, preprocessedTemplates).first << endl;


                    //visualize resamped and rotated and translated points for fun, comment out when not debugging
                    /*
                    for (int i = 0; i < rotated.size(); i++) {
                        resampledVisualization.push_back(sf::Vertex(sf::Vector2f(translated[i].x, translated[i].y), sf::Color(0, 0, 0)));
                        //cout << translated[i].x << " " << translated[i].y << endl;
                    }*/
                    break;
                }
                // if mouse btn released outside of clear btn, do not clear
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
        }*/
        window.display();

    }

    

    return 0;
}
