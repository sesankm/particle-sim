#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const float kGravity    = 0.0001;
const int kParticleMaxX = 600;
const int kParticleMaxY = 300;
const int kParticleMinX = 200;

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
        velocity = sf::Vector2f((rand() % 100 - 50) / 1000.0f, kGravity);
        bounce_dampner = 1.0;
        roll_dampner = 1.0;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, rand() % 155 + 100, rand() % 155 + 100));
        shape.setPosition(position);
    }

    void update_vy() {
        if(position.y + velocity.y <= kParticleMaxY - (2 * radius)) {
            velocity.y += kGravity;
        } else if(position.y >= kParticleMaxY - (2 * radius)) {
            velocity.y = 0;
        }
        // else if(velocity.y >= 0) {
        //     velocity.y *= -1 * bounce_dampner;
        //     if(bounce_dampner > 0) {
        //         bounce_dampner -= 0.1;
        //     }
        // }
    }

    void update_vx() {
        if(position.x + velocity.x >= kParticleMaxX - (2 * radius) || position.x + velocity.x < kParticleMinX) {
            velocity.x *= -1;
        }
        // if(position.y >= kParticleMaxY - (2 * radius)) {
        //     if(roll_dampner > 0) {
        //         roll_dampner -= 0.00000003;
        //         velocity.x *= roll_dampner;
        //     }
        // }
    }

    void update_color(int tick) {
        if(tick % 25 == 0) {
            sf::Color color = shape.getFillColor();
            if(color.r > 0) {
                color.r -= 1;
            }
            shape.setFillColor(color);
        }
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

        sf::Vector2f p1_norm = norm;
        sf::Vector2f p2_norm = norm;
        p1.position += p1_norm;
        p2.position -= p2_norm;
    }
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
    std::vector<Particle> p;
    int tick;

    while (window.isOpen()) {
        tick++;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        if(tick == 25) {
            if(p.size() < 500)
                p.push_back(Particle());
            tick = 0;
        }

        for(int i = 0; i < p.size(); i++) {
            for(int j = 0; j < p.size(); j++) {
                if(i != j) {
                    check_collision(p[i], p[j]);
                }
            }
        }

        for(int i = 0; i < p.size(); i++) {
            p[i].render(window);
            p[i].update_color(tick);
        }
        window.display();
    }
    return 0;
}

