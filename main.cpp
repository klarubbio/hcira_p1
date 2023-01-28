#include <SFML/Graphics.hpp>

int main()
{
    int width = 400;
    int height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");

    window.setFramerateLimit(100);

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
                drawing = true;
                last = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                vertices.append(sf::Vertex(last, sf::Color(0, 0, 0))); //track actual points
                brush.setPosition(last);
                texture.draw(brush);
                texture.display();
                break;
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
                drawing = false;
                break;
            }
            
        }

        window.clear(sf::Color(255,255,255));
        window.draw(sprite);
        //draw only actual points recorded
        //window.draw(vertices);
        window.display();
    }

    return 0;
}