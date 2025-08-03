#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int   WIN_H    = 800;
const int   WIN_W    = 800;
const sf::Vector2f GRAV(0.0f, 9.8f);

class Particle {
public:
    float radius;
    sf::Vector2f pos;
    sf::Vector2f prev_pos;
    sf::Vector2f accel;

    sf::CircleShape shape;

    Particle() {
        radius = 40.0f;

        accel = {};
        pos = sf::Vector2f(rand() % 40 + 340, 200.0f);
        prev_pos = pos;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setPosition(pos);
    }

    void apply_grav() {
        accel += GRAV;
    }

    void update_pos(float dt) {
        sf::Vector2f vel = prev_pos - pos;
        prev_pos = pos;
        pos = pos + vel + accel * (dt * dt);
        accel = {};
    }

    void check_boundary() {
        if(pos.y + radius * 2 > WIN_H) {
            pos.y -= (pos.y + radius * 2 - WIN_H);
        }
        if(pos.x < 0) {
            pos.x += (0 - pos.x);
        } else if(pos.x + radius * 2 > WIN_W) {
            pos.x -= (pos.x + radius * 2 - WIN_W);
        }
    }

    void render(sf::RenderWindow& window) {
        shape.setPosition(pos);
        window.draw(shape);
    }
};

void check_collision(Particle& p1, Particle& p2) {
    sf::Vector2f diff = p1.pos - p2.pos;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if(dist < p1.radius + p2.radius) {
        sf::Vector2f norm = diff / dist;
        float delta = p1.radius + p2.radius - dist;
        p1.pos += 0.5f * delta * norm;
        p2.pos -= 0.5f * delta * norm;
    }
}


void update_positions(std::vector<Particle>& particles) {
    int substeps = 2;
    const float sub_dt = 1.0f / static_cast<float>(substeps);

    for(int s = 0; s < substeps; s++) {
        for(int i = 0; i < particles.size(); i++) {
            particles[i].apply_grav();
            particles[i].update_pos(sub_dt);
            for(int j = 0; j < particles.size(); j++) {
                if(i != j) {
                    check_collision(particles[i], particles[j]);
                }
            }
            particles[i].check_boundary();
        }
    }
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
    std::vector<Particle> p;

    p.push_back(Particle());
    window.setFramerateLimit(240);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::A) {
                    p.push_back(Particle());
                }
            }
        }

        window.clear();
        update_positions(p);
        std::for_each(p.begin(), p.end(), [&window](Particle& pa) {
            pa.render(window);
        });
        window.display();
    }
    return 0;
}

