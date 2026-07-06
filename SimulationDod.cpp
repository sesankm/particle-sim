#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <thread>

#include "grid.hpp"
#include "constants.hpp"

std::mutex mut;

void epoch_grid(Grid& p, sf::RenderWindow& window);
void render(sf::RenderWindow& window, Grid& grid);


void apply_grav(Grid& grid);
void check_boundary(Grid& grid);

void check_collision(Grid& grid);
void check_collision_seg(Grid&, int, int);
void check_collision_cell(Grid& grid, int curr_cell, int other_cell);

void update_pos(Grid& grid);

int grid_index(int x, int y);
void add_grid_particle(Grid& grid);

int grid_index(int x, int y) {
    int grid_row = static_cast<int>(y) / CELL_W;
    int grid_col = static_cast<int>(x) / CELL_W;
    return grid_row * GRID_COLS + grid_col;
}

void add_grid_particle(Grid& grid) {
    float x = rand() % WIN_W;
    float y = 50;

    grid.pos_x.push_back(x);
    grid.pos_y.push_back(y);

    grid.prev_x.push_back(x);
    grid.prev_y.push_back(y);

    grid.accel_x.push_back(0);
    grid.accel_y.push_back(0);

    grid.cells[grid_index(x, y)].push_back(grid.num_particles++);
}

void apply_grav(Grid& grid) {
    for (auto& pa : grid.accel_y) {
        pa += G_FORCE;
    }
}

void check_boundary(Grid& grid) {
    for (int pi = 0; pi < grid.pos_x.size(); pi++) {
        if (grid.pos_x[pi] < 0)
            grid.pos_x[pi] += (0 - grid.pos_x[pi]);
        else if (grid.pos_x[pi] + PART_R * 2 > WIN_W) 
            grid.pos_x[pi] -= (grid.pos_x[pi] + PART_R * 2 - WIN_W);
        if (grid.pos_y[pi] + PART_R * 2 > WIN_H)
            grid.pos_y[pi] -= (grid.pos_y[pi] + PART_R * 2 - WIN_H);
    }
}

void check_collision_cell(Grid& grid, int curr_cell, int other_cell) {
    for (int pi : grid.cells[curr_cell]) {
        for (int opi : grid.cells[other_cell]) {
            if (pi == opi) { continue; }
            float x = grid.pos_x[pi];
            float y = grid.pos_y[pi];

            float ox = grid.pos_x[opi];
            float oy = grid.pos_y[opi];

            float diff_x = x - ox;
            float diff_y = y - oy;
            float dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);

            if (dist < PART_R * 2) {
                float norm_x = diff_x / dist;
                float norm_y = diff_y / dist;
                float delta = PART_R * 2 - dist;

                std::lock_guard<std::mutex> lock(mut);
                grid.pos_x[opi] -= 0.5f * delta * norm_x;
                grid.pos_y[opi] -= 0.5f * delta * norm_y;
                grid.pos_x[pi]   += 0.5f * delta * norm_x;
                grid.pos_y[pi]   += 0.5f * delta * norm_y;
            }
        }
    }
}

void check_collision_seg(Grid& grid, int seg_start, int seg_end) {
    for (int ci = seg_start; ci < seg_end; ci++) {
        check_collision_cell(grid, ci, ci);

        if (ci + GRID_COLS < grid.cells.size())
            check_collision_cell(grid, ci, ci + GRID_COLS);
        if (ci - GRID_COLS > 0)
            check_collision_cell(grid, ci, ci - GRID_COLS);

        if (GRID_COLS % ci != 0) {
            if (ci > 0)
                check_collision_cell(grid, ci, ci - 1);
            if (ci - 1 - GRID_COLS > 0)
                check_collision_cell(grid, ci, ci - 1 - GRID_COLS);
            if (ci - 1 + GRID_COLS < grid.cells.size())
                check_collision_cell(grid, ci, ci - 1 + GRID_COLS);
        }
    }
}

void check_collision(Grid& grid) {
    std::vector<std::thread> threads;
    int seg_size = grid.cells.size() / N_THREADS;
    for (int i = 0; i < N_THREADS; i++) {
        int seg_start = i * seg_size;
        int seg_end = i == N_THREADS - 1 ? grid.cells.size() : seg_start + seg_size;
        threads.emplace_back(check_collision_seg, std::ref(grid), seg_start, seg_end);
    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& t){t.join();});
}

void update_pos(Grid& grid) {
    float dt = 1.0f / N_TS;
    sf::Vector2f vel;
    for (int pi = 0; pi < grid.pos_x.size(); pi++) {
        vel = {grid.pos_x[pi] - grid.prev_x[pi], grid.pos_y[pi] - grid.prev_y[pi]};

        grid.prev_x[pi] = grid.pos_x[pi];
        grid.prev_y[pi] = grid.pos_y[pi];

        grid.pos_x[pi] += (vel.x + grid.accel_x[pi] * dt * dt);
        grid.pos_y[pi] += (vel.y + grid.accel_y[pi] * dt * dt);

        grid.accel_x[pi] = 0;
        grid.accel_y[pi] = 0;

        int ind = grid_index(grid.pos_x[pi], grid.pos_y[pi]);

        if (ind < 0)
            ind = 0;
        else if (ind >= GRID_COLS * GRID_ROWS)
            ind = GRID_COLS * GRID_ROWS - 1;

        grid.cells[ind].push_back(pi);
    }
}


void render(sf::RenderWindow& window, Grid& grid) {
    sf::CircleShape foo(PART_R);
    for (int pi = 0; pi < grid.pos_x.size(); pi++) {
        foo.setPosition({grid.pos_x[pi], grid.pos_y[pi]});
        window.draw(foo);
    }
}

void epoch_grid(Grid& p, sf::RenderWindow& window) {
    for (int i = 0; i < N_TS; i++) {
        apply_grav(p);
        check_boundary(p);
        check_collision(p);

        for(auto& cell : p.cells) { cell.clear(); }

        update_pos(p);
        render(window, p);
    }
}
