#ifndef GRID_H
#define GRID_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <vector>
#include "constants.hpp"
#include "threadpool.hpp"

struct Grid {
    int num_particles { 0 };

    std::vector<float> pos_x;
    std::vector<float> pos_y;

    std::vector<float> prev_x;
    std::vector<float> prev_y;

    /*
      Keep track of horizontal and vertical acceleration to apply to
      each particle for every frame.
     */
    std::vector<float> accel_x;
    std::vector<float> accel_y;

    /*
      Instead of storing positions directly in nested structures,
      store indexes to compare particles in adjacent cells.
     */
    std::vector<std::vector<int>> cells { GRID_COLS * GRID_ROWS, std::vector<int>{} };
    ThreadPool threadPool;

public:
    Grid() {
        pos_x.reserve(N_PARTS);
        pos_y.reserve(N_PARTS);

        prev_x.reserve(N_PARTS);
        prev_y.reserve(N_PARTS);

        accel_x.reserve(N_PARTS);
        accel_y.reserve(N_PARTS);

        std::for_each(cells.begin(), cells.end(), [](auto& vec){ vec.reserve(N_PARTS / (GRID_ROWS * GRID_COLS)); });
    }
    void epoch_grid(sf::RenderWindow&);
    void add_particle();

private:
    void render(sf::RenderWindow&);
    void apply_grav();
    void check_boundary();
    void check_collision();
    void check_collision_seg(int, int);
    void check_collision_cell(int, int);
    void update_pos();
    int grid_index(int, int);
};

#endif
