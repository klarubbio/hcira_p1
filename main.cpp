#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    int width = 400;
    int height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");
    sf::Texture clearBtn;
    sf::Texture clearBtnPressed;
    sf::Sprite clearBtnSprite;

    window.setFramerateLimit(100);

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
    sf::VertexArray vertices;
    vertices.setPrimitiveType(sf::Points);

    //visualizes points (makes everything look connected instead of a bunch of small dots)
    sf::RenderTexture texture;
    texture.create(width, height);
    texture.clear(sf::Color::White);

    sf::Sprite sprite;
    sprite.setTexture(texture.getTexture(), true);

    const float brush_size = 8;
    sf::CircleShape brush(brush_size, 8);
    brush.setOrigin(brush_size, brush_size);
    brush.setFillColor(sf::Color(150, 150, 150));

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
                    drawing = true;
                    last = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    vertices.append(sf::Vertex(last, sf::Color(0, 0, 0))); //track actual points
                    brush.setPosition(last);
                    texture.draw(brush);
                    texture.display();
                    break;
                }
            }
            //connect additional points of contact
            else if (event.type == sf::Event::MouseMoved && drawing) {
                const sf::Vector2f next(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                vertices.append(sf::Vertex(next, sf::Color(0, 0, 0))); //track actual points
                brush.setPosition(next);
                texture.draw(brush);
                texture.display();
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
                    texture.clear(sf::Color::White);
                }
            }
            
        }

        window.clear(sf::Color(255,255,255));
        window.draw(sprite);
        window.draw(clearBtnSprite);
        //draw only actual points recorded
        //window.draw(vertices);
        window.display();
    }

    return 0;
}