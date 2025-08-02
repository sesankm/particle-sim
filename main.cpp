#include <SFML/Graphics.hpp>
#include <iostream>

const float kGravity = 0.0001;
const int kWinWidth  = 800;
const int kWinHeight = 600;

class Particle {
public:
    float radius;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    float bounce_dampner;

    Particle() {
        radius = 8.0f;
        position = sf::Vector2f(388.0f, 100.0f);
        velocity = sf::Vector2f(0.0f, kGravity);
        bounce_dampner = 1.0;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setPosition(position);
    }

    void update_velocity() {
        if(position.y + velocity.y <= kWinHeight - (2 * radius)) {
            velocity.y += kGravity;
        } else if(velocity.y > 0) {
            velocity.y *= -1;
            if(bounce_dampner > 0) {
                bounce_dampner -= 0.2;
            }
        }
    }

    void update() {
        update_velocity();
        position += (velocity * bounce_dampner);
        shape.setPosition(position);
    }

    void render(sf::RenderWindow& window) {
        update();
        window.draw(shape);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    Particle p;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        p.render(window);
        window.display();
    }
    return 0;
}

