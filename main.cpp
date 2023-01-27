#include <SFML/Graphics.hpp>

int main()
{
    int width = 400;
    int height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    window.setFramerateLimit(200);

    sf::RenderTexture texture;
    texture.create(width, height);
    texture.clear(sf::Color::White);

    sf::Sprite sprite;
    sprite.setTexture(texture.getTexture(), true);

    const float brush_size = 10;
    sf::CircleShape brush(brush_size, 10);
    brush.setOrigin(brush_size, brush_size);
    brush.setFillColor(sf::Color(150,150,150));

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
            else if (event.type == sf::Event::MouseButtonPressed) {
                printf("button pressed");
                drawing = true;
                last = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                brush.setPosition(last);
                texture.draw(brush);
                texture.display();
                break;
            }
            else if (event.type == sf::Event::MouseMoved && drawing) {
                printf("mouse moved");
                const sf::Vector2f next(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                brush.setPosition(next);
                texture.draw(brush);
                texture.display();
                break;
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                drawing = false;
                break;
            }
        }

        sprite.setTexture(texture.getTexture(), true);
        window.clear(sf::Color(64,64,64));
        window.draw(sprite);
        window.display();
    }

    return 0;
}