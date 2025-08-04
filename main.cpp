#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Constants.hpp"
#include "Particle.cpp"
#include "Simulation.cpp"

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
    window.setFramerateLimit(120);
    Simulation sim;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            }
        }

        if(sim.num_particles < 500) {
            sim.add_particle();
        }

        window.clear();
        sim.update_positions();
        sim.render(window);
        window.display();
    }
    return 0;
}

