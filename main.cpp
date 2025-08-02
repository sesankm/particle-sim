#include <SFML/Graphics.hpp>
#include <iostream>

const float kGravity = 0.098;
const int kWinWidth  = 800;
const int kWinHeight = 600;

class Particle {
public:
    float radius;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    Particle() {
        radius = 8.0f;
        position = sf::Vector2f(388.0f, 100.0f);
        velocity = sf::Vector2f(0.0f, kGravity * (kWinHeight - position.y));

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setPosition(position);
    }

    void update_velocity() {
        if(velocity.y > 0 && position.y + velocity.y >= kWinHeight - radius) {
            velocity.y = -kGravity * 0.9f;
        } else if(velocity.y >= 0) {
            velocity.y = kGravity * (position.y / 500);
        } else if(velocity.y < 0) {
            velocity.y += 0.0001f;
        } 
    }

    void update() {
        update_velocity();
        position += velocity;
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

