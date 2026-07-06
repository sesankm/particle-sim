#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "constants.hpp"
#include "grid.hpp"

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "SFML window");
    window.setFramerateLimit(120);

    Grid g;

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (g.num_particles < N_PARTS) {
            g.add_particle();
        }

        sf::Time elapsedTime = clock.restart();
        float deltaTime = elapsedTime.asSeconds();
        float fps = 1.f / deltaTime;

        if (fps < 60 && g.num_particles > 500) {
            std::cout << g.num_particles << ", FPS: " << fps << "\n";
            break;
        }

        window.clear();
        g.epoch_grid(window);
        window.display();
    }

    return 0;
}
