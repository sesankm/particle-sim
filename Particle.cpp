#include "Constants.hpp"

const sf::Vector2f GRAV(0.0f, 0.098f);

class Particle {
public:
    float radius;
    sf::Vector2f pos;
    sf::Vector2f prev_pos;
    sf::Vector2f accel;

    sf::CircleShape shape;

    Particle() {
        radius = 10.0f;

        accel = sf::Vector2f(0.0f, 0.0f);
        pos = sf::Vector2f(rand() % 100 + 300, 200.0f);
        prev_pos = pos;

        shape = sf::CircleShape(radius);
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setPosition(pos);
    }

    void apply_grav() {
        accel += GRAV;
    }

    void update_pos(float dt) {
        sf::Vector2f vel = pos - prev_pos;
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
