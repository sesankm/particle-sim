#include <thread>

#include "grid.hpp"
#include "constants.hpp"

std::mutex mut;

int Grid::grid_index(int x, int y) {
    int grid_row = static_cast<int>(y) / CELL_W;
    int grid_col = static_cast<int>(x) / CELL_W;
    return grid_row * GRID_COLS + grid_col;
}

void Grid::add_particle() {
    float x = rand() % WIN_W;
    float y = 50;

    pos_x.push_back(x);
    pos_y.push_back(y);

    prev_x.push_back(x);
    prev_y.push_back(y);

    accel_x.push_back(0);
    accel_y.push_back(0);

    cells[grid_index(x, y)].push_back(num_particles++);
}

void Grid::apply_grav() {
    for (auto& pa : accel_y) {
        pa += G_FORCE;
    }
}

void Grid::check_boundary() {
    for (int pi = 0; pi < pos_x.size(); pi++) {
        if (pos_x[pi] < 0)
            pos_x[pi] += (0 - pos_x[pi]);
        else if (pos_x[pi] + PART_R * 2 > WIN_W) 
            pos_x[pi] -= (pos_x[pi] + PART_R * 2 - WIN_W);
        if (pos_y[pi] + PART_R * 2 > WIN_H)
            pos_y[pi] -= (pos_y[pi] + PART_R * 2 - WIN_H);
    }
}

void Grid::check_collision_cell(int curr_cell, int other_cell) {
    for (int pi : cells[curr_cell]) {
        for (int opi : cells[other_cell]) {
            if (pi == opi) { continue; }
            float x = pos_x[pi];
            float y = pos_y[pi];

            float ox = pos_x[opi];
            float oy = pos_y[opi];

            float diff_x = x - ox;
            float diff_y = y - oy;
            float dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);

            if (dist < PART_R * 2) {
                float norm_x = diff_x / dist;
                float norm_y = diff_y / dist;
                float delta = PART_R * 2 - dist;

                std::lock_guard<std::mutex> lock(mut);
                pos_x[opi] -= 0.5f * delta * norm_x;
                pos_y[opi] -= 0.5f * delta * norm_y;
                pos_x[pi]   += 0.5f * delta * norm_x;
                pos_y[pi]   += 0.5f * delta * norm_y;
            }
        }
    }
}

void Grid::check_collision_seg(int seg_start, int seg_end) {
    for (int ci = seg_start; ci < seg_end; ci++) {
        check_collision_cell(ci, ci);

        if (ci + GRID_COLS < cells.size())
            check_collision_cell(ci, ci + GRID_COLS);
        if (ci - GRID_COLS >= 0)
            check_collision_cell(ci, ci - GRID_COLS);

        if (GRID_COLS % ci != 0) {
            if (ci > 0)
                check_collision_cell(ci, ci - 1);
            if (ci - 1 - GRID_COLS >= 0)
                check_collision_cell(ci, ci - 1 - GRID_COLS);
            if (ci - 1 + GRID_COLS < cells.size())
                check_collision_cell(ci, ci - 1 + GRID_COLS);
        }
    }
}

void Grid::check_collision() {
    std::vector<std::thread> threads;
    int seg_size = cells.size() / N_THREADS;
    for (int i = 0; i < N_THREADS; i++) {
        int seg_start = i * seg_size;
        int seg_end = i == N_THREADS - 1 ? cells.size() : seg_start + seg_size;
        threads.emplace_back(&Grid::check_collision_seg, this, seg_start, seg_end);
    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& t){t.join();});
}

void Grid::update_pos() {
    float dt = 1.0f / N_TS;
    sf::Vector2f vel;
    for (int pi = 0; pi < pos_x.size(); pi++) {
        vel = {pos_x[pi] - prev_x[pi], pos_y[pi] - prev_y[pi]};

        prev_x[pi] = pos_x[pi];
        prev_y[pi] = pos_y[pi];

        pos_x[pi] += (vel.x + accel_x[pi] * dt * dt);
        pos_y[pi] += (vel.y + accel_y[pi] * dt * dt);

        accel_x[pi] = 0;
        accel_y[pi] = 0;

        int ind = grid_index(pos_x[pi], pos_y[pi]);

        if (ind < 0)
            ind = 0;
        else if (ind >= GRID_COLS * GRID_ROWS)
            ind = GRID_COLS * GRID_ROWS - 1;

        cells[ind].push_back(pi);
    }
}


void Grid::render(sf::RenderWindow& window) {
    sf::CircleShape foo(PART_R);
    for (int pi = 0; pi < pos_x.size(); pi++) {
        foo.setPosition({pos_x[pi], pos_y[pi]});
        window.draw(foo);
    }
}

void Grid::epoch_grid(sf::RenderWindow& window) {
    for (int i = 0; i < N_TS; i++) {
        apply_grav();
        check_boundary();
        check_collision();

        for(auto& cell : cells) { cell.clear(); }

        update_pos();
    }
    render(window);
}
