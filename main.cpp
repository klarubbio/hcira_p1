#include <SFML/Graphics.hpp>
#include <iostream>
#include "TemplateMap.cpp"

int main()
{

    // test out TemplateMap
    TemplateMap templateMap;
    vector<Point> testPoints1;
    testPoints1.push_back(Point(0, 0));
    testPoints1.push_back(Point(3, 2));
    testPoints1.push_back(Point(8, 3));
    testPoints1.push_back(Point(6, 7));
    testPoints1.push_back(Point(5, 4));
    templateMap.addTemplate("star", testPoints1);

    vector<Point> testPoints2;
    testPoints2.push_back(Point(3, 5));
    testPoints2.push_back(Point(2, 6));
    testPoints2.push_back(Point(5, 3));
    testPoints2.push_back(Point(7, 7));
    testPoints2.push_back(Point(0, 9));
    templateMap.addTemplate("star", testPoints2);

    vector<Point> testPoints3;
    testPoints3.push_back(Point(4, 5));
    testPoints3.push_back(Point(7, 5));
    testPoints3.push_back(Point(0, 1));
    testPoints3.push_back(Point(8, 3));
    testPoints3.push_back(Point(0, 7));
    templateMap.addTemplate("rectangle", testPoints3);

    templateMap.printTemplateMap();
    
    
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
    sf::VertexArray vertices(sf::LineStrip);

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
                    vertices.append(sf::Vertex(last, sf::Color(0, 0, 0)));
                    break;
                }
            }
            //connect additional points of contact
            else if (event.type == sf::Event::MouseMoved && drawing) {
                const sf::Vector2f next(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                vertices.append(sf::Vertex(next, sf::Color(0, 0, 0)));
                last = next;
                break;
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (drawing) {
                    drawing = false;
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
                }
            }
            
        }

        window.clear(sf::Color(255,255,255));
        window.draw(clearBtnSprite);
        //draw only actual points recorded and lines in between
        window.draw(vertices);
        window.display();
    }

    return 0;
}
