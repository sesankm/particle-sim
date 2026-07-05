#ifndef GRID_H
#define GRID_H

#include <vector>

struct Grid {
    std::vector<float> pos_x;
    std::vector<float> pos_y;

    std::vector<float> prev_x;
    std::vector<float> prev_y;

    std::vector<float> accel_x;
    std::vector<float> accel_y;

    int num_particles;
};

#endif
