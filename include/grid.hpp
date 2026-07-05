#ifndef GRID_H
#define GRID_H

#include "constants.hpp"
#include <array>
#include <vector>

struct Grid {
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> pos_x;
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> pos_y;

    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> prev_x;
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> prev_y;

    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> accel_x;
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> accel_y;

    int num_particles;
};

#endif
