#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Constants.hpp"
#include "Particle.cpp"
#include "Simulation.cpp"

#include "Components.cpp"

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
    window.setFramerateLimit(120);

    int num_particles = 0;
    Position p;
    Figure f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                add_particle(p, f);
            }
        }

        if (p.x.size() < 1700) {
            add_particle(p, f);
        }

        window.clear();
        epoch(f, p, num_particles, window);
        window.display();
    }
    return 0;
}

// int main() {
//     srand(time(0));
//     sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
//     window.setFramerateLimit(120);
//     Simulation sim;

//     while (window.isOpen()) {
//         while (const std::optional event = window.pollEvent()) {
//             if (event->is<sf::Event::Closed>()) {
//                 window.close();
//             }
//             if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//                 sim.add_particle();
//             }
//         }

//         if(sim.num_particles < N_CELLS) {
//             sim.add_particle();
//         }

//         window.clear();
//         sim.update_positions();
//         sim.render(window);
//         window.display();
//     }
//     return 0;
// }

