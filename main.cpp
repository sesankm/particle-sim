#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const float kGravity    = 3.0f;
const int kParticleMaxX = 800;
const int kParticleMaxY = 600;
const int kParticleMinX = 0;

class Particle {
public:
    float radius;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    float bounce_dampner;
    float roll_dampner;

    Particle() {
        radius = 10.0f;
        position = sf::Vector2f(388.0f, 200.0f);
        velocity = sf::Vector2f((rand() % 100 - 50) / 1000.0f, kGravity);
        bounce_dampner = 2.0;
        roll_dampner = 1.0;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, rand() % 155 + 100, rand() % 155 + 100));
        shape.setPosition(position);
    }

    void update_vy() {
        if(position.y + radius > kParticleMaxY) {
            velocity.y = -2.0f * bounce_dampner;
        } else {
            if(velocity.y < 0) {
                velocity.y += 0.10;
            } else if(position.y + velocity.y <= kParticleMaxY - radius) {
                velocity.y += kGravity / (radius * radius);
            }
        }
    }

    void update_vx() {
        if(position.x + velocity.x >= kParticleMaxX - (2 * radius) || position.x + velocity.x < kParticleMinX) {
            velocity.x *= -1;
        }
    }

    void update_color() {
        sf::Color color = shape.getFillColor();
        if(color.r > 0) {
            color.r -= 1;
        }
        shape.setFillColor(color);
    }

    void update() {
        update_vy();
        update_vx();
        position += velocity;
        shape.setPosition(position);
    }

    void render(sf::RenderWindow& window) {
        update();
        window.draw(shape);
    }
};

void check_collision(Particle& p1, Particle& p2) {
    sf::Vector2f diff = p1.position - p2.position;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if(distance < p1.radius + p2.radius) {
        sf::Vector2f norm = (diff / (distance == 0.0f ? 0.0001f : distance));
        float delta = ((p1.radius - p2.radius) * 2) - distance;
        p1.position += norm;
        p2.position -= norm;
    }
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
    std::vector<Particle> p;
    window.setFramerateLimit(120);
    int tick;

    while (window.isOpen()) {
        tick++;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        if(p.size() < 1000)
            p.push_back(Particle());

        for(int i = 0; i < p.size(); i++) {
            for(int j = 0; j < p.size(); j++) {
                if(i != j) {
                    check_collision(p[i], p[j]);
                }
            }
        }

        for(int i = 0; i < p.size(); i++) {
            p[i].render(window);
            p[i].update_color();
        }
        window.display();
    }
    return 0;
}

