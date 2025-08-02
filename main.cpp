#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

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
    float roll_dampner;

    Particle() {
        radius = 8.0f;
        position = sf::Vector2f(388.0f, 100.0f);
        velocity = sf::Vector2f((rand() % 50 + 1) / 1000.0f, kGravity);
        bounce_dampner = 1.0;
        roll_dampner = 1.0;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setPosition(position);
    }

    void update_vy() {
        if(position.y + velocity.y <= kWinHeight - (2 * radius)) {
            velocity.y += kGravity;
        } else if(velocity.y > 0.3) {
            velocity.y *= -1;
            if(roll_dampner > 0) {
                roll_dampner -= 0.2;
            }
            if(bounce_dampner > 0) {
                bounce_dampner -= 0.2;
            }
        }
    }

    void update_vx() {
        if(position.x + velocity.x >= kWinWidth - (2 * radius) || position.x + velocity.x <= 0) {
            velocity.x *= -1;
        }
        if(position.y <= kWinHeight - (2 * radius)) {
        }
    }

    void update() {
        update_vy();
        update_vx();

        sf::Vector2f curr_v(velocity.x * roll_dampner, velocity.y * bounce_dampner);
        position += curr_v;
        shape.setPosition(position);
    }

    void render(sf::RenderWindow& window) {
        update();
        window.draw(shape);
    }
};

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    Particle p;
    Particle p2;
    Particle p3;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        p.render(window);
        p2.render(window);
        p3.render(window);
        window.display();
    }
    return 0;
}

