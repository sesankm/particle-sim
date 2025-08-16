#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Constants.hpp"
#include "./oop_sim/Particle.cpp"
#include "./oop_sim/Simulation.cpp"

#include "SimulationDod.cpp"

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
    window.setFramerateLimit(120);

    int num_particles = 0;
    Grid p;
    p.num_particles = 0;

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (num_particles < N_CELLS) {
            add_grid_particle(p);
            p.num_particles++;
        }

        sf::Time elapsedTime = clock.restart();
        float deltaTime = elapsedTime.asSeconds();
        float fps = 1.f / deltaTime;

        if (fps < 60 && p.num_particles > 500) {
            std::cout << p.num_particles << ", FPS: " << fps << "\n";
            break;
        }

        window.clear();
        epoch_grid(p, num_particles, window);
        window.display();

        // DATA ORIENTED GRID BASED 
        // 2516 particles before droping below 60 FPS
    }

    return 0;
}

// int main() {
//     srand(time(0));
//     sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
//     window.setFramerateLimit(120);
//     Simulation sim;

//     sf::Clock clock;
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

//         sf::Time elapsedTime = clock.restart();
//         float deltaTime = elapsedTime.asSeconds();
//         float fps = 1.f / deltaTime;

//         if (fps < 60 && sim.num_particles > 500) {
//             std::cout << sim.num_particles << ", FPS: " << fps << "\n";
//             break;
//         }

//         // OOP GRID BASED 
//         // 1564 particles before dropping below 60 FPS

//         window.clear();
//         sim.update_positions();
//         sim.render(window);
//         window.display();
//     }
//     return 0;
// }
