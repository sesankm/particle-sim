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
        shape.setFillColor(sf::Color(rand() % 155 + 100, rand() % 155 + 100, 255));
        shape.setPosition(position);
    }

    void update_vy() {
        if(position.y + velocity.y <= kWinHeight - (2 * radius)) {
            velocity.y += kGravity;
        } else if(velocity.y >= 0) {
            velocity.y *= -1 * bounce_dampner;
            if(bounce_dampner > 0) {
                bounce_dampner -= 0.1;
            }
        }
    }

    void update_vx() {
        if(position.x + velocity.x >= kWinWidth - (2 * radius) || position.x + velocity.x <= 0) {
            velocity.x *= -1;
        }
        if(position.y >= kWinHeight - (2 * radius)) {
            if(roll_dampner > 0) {
                roll_dampner -= 0.00000003;
                velocity.x *= roll_dampner;
            }
        }
    }

    void update() {
        update_vy();
        update_vx();
        position += velocity;
        shape.setPosition(position);
    }

    void render(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

void check_collision(Particle& p1, Particle& p2) {
    sf::Vector2f diff = p1.position - p2.position;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if(distance <= 8.0f) {
        sf::Vector2f norm = (diff / (distance == 0.0f ? 0.0001f : distance));
        float delta = (8.0f * 2) - distance;
        norm *= (delta * 0.002f);
        p1.velocity += norm;
        p2.velocity -= norm;
    }
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
    Particle p;
    Particle p2;
    Particle p3;
    int tick;

    std::vector<Particle> particles;

    while (window.isOpen()) {
        tick++;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();

        if(tick == 25) {
            check_collision(p, p2);
            check_collision(p, p3);
            check_collision(p2, p3);
            tick = 0;
        }
        p.update();
        p2.update();
        p3.update();
        p.render(window);
        p2.render(window);
        p3.render(window);
        window.display();
    }
    return 0;
}

