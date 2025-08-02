#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const float kGravity    = 0.05f;
const int kParticleMaxX = 600;
const int kParticleMaxY = 500;
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
        radius = 7.0f;
        position = sf::Vector2f(388.0f, 200.0f);
        velocity = sf::Vector2f((rand() % 100 - 50) / 1000.0f, kGravity);
        bounce_dampner = 1.0;
        roll_dampner = 1.0;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, rand() % 155 + 100, rand() % 155 + 100));
        shape.setPosition(position);
    }

    void update_vy() {
        if(position.y >= kParticleMaxY - (2 * radius)) {
            velocity.y = 0;
        } 
    }

    void update_vx() {
        if(position.x + velocity.x >= kParticleMaxX - (2 * radius) || position.x + velocity.x < kParticleMinX) {
            velocity.x *= -1;
        }
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
    if(distance <= p1.radius + p2.radius) {
        sf::Vector2f norm = (diff / (distance == 0.0f ? 0.0001f : distance));
        float delta = ((p1.radius - p2.radius) * 2) - distance;

        sf::Vector2f p1_norm = norm;
        sf::Vector2f p2_norm = norm;

        if(p1.position.x + p1_norm.x > kParticleMaxX - p1.radius || p1.position.x + p1_norm.x < kParticleMinX + p1.radius) {
            p1_norm.y += std::abs(p1_norm.x);
            p1_norm.x = 0;
            p2_norm.x *= 2;
        }
        if(p2.position.x - p2_norm.x > kParticleMaxX - p2.radius || p2.position.x - p2_norm.x < kParticleMinX + p2.radius) {
            p2_norm.y += std::abs(p2_norm.x);
            p2_norm.x = 0;
            p1_norm.x *= 2;
        }
        if(p1.position.y + p1_norm.y > kParticleMaxY - p1.radius) {
            p1_norm.y = 0;
            p2_norm.y *= 2;
        }
        if(p2.position.y - p2_norm.y > kParticleMaxY - p2.radius) {
            p2_norm.y = 0;
            p1_norm.y *= 2;
        }

        p1.position += p1_norm;
        p2.position -= p2_norm;
    }
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
    std::vector<Particle> p;
    int tick;

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(406, 405));
    rect.setPosition(sf::Vector2f(202, 100));
    rect.setFillColor(sf::Color(18, 18, 18));
    while (window.isOpen()) {
        tick++;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(rect);
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

