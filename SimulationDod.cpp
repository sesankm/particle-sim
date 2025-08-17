#include <thread>

constexpr int GRID_ROWS = WIN_H / CELL_W;
constexpr int GRID_COLS = WIN_W / CELL_W;

std::mutex mut;

struct Grid {
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> pos_x;
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> pos_y;

    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> prev_x;
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> prev_y;

    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> accel_x;
    std::array<std::vector<float>, GRID_COLS * GRID_ROWS> accel_y;

    int num_particles;
};

void move_particle(Grid& grid, int old_grid_ind, int new_grid_ind, int particle_ind) {
    grid.pos_x[new_grid_ind].push_back(grid.pos_x[old_grid_ind][particle_ind]);
    grid.pos_y[new_grid_ind].push_back(grid.pos_y[old_grid_ind][particle_ind]);
    grid.prev_x[new_grid_ind].push_back(grid.prev_x[old_grid_ind][particle_ind]);
    grid.prev_y[new_grid_ind].push_back(grid.prev_y[old_grid_ind][particle_ind]);
    grid.accel_x[new_grid_ind].push_back(grid.accel_x[old_grid_ind][particle_ind]);
    grid.accel_y[new_grid_ind].push_back(grid.accel_y[old_grid_ind][particle_ind]);

    grid.pos_x[old_grid_ind].erase(grid.pos_x[old_grid_ind].begin() + particle_ind);
    grid.pos_y[old_grid_ind].erase(grid.pos_y[old_grid_ind].begin() + particle_ind);
    grid.prev_x[old_grid_ind].erase(grid.prev_x[old_grid_ind].begin() + particle_ind);
    grid.prev_y[old_grid_ind].erase(grid.prev_y[old_grid_ind].begin() + particle_ind);
    grid.accel_x[old_grid_ind].erase(grid.accel_x[old_grid_ind].begin() + particle_ind);
    grid.accel_y[old_grid_ind].erase(grid.accel_y[old_grid_ind].begin() + particle_ind);
}

int grid_index(int x, int y) {
    int grid_row = static_cast<int>(y) / CELL_W;
    int grid_col = static_cast<int>(x) / CELL_W;
    return grid_row * GRID_COLS + grid_col;
}

void add_grid_particle(Grid& grid) {
    float x = rand() % WIN_W;
    float y = 50;
    int ind = grid_index(x, y);

    grid.pos_x[ind].push_back(x);
    grid.pos_y[ind].push_back(y);

    grid.prev_x[ind].push_back(x);
    grid.prev_y[ind].push_back(y);

    grid.accel_x[ind].push_back(0);
    grid.accel_y[ind].push_back(0);
}

void apply_grav(Grid& grid) {
    for (auto& cell : grid.accel_y) {
        for (auto& pa : cell) {
            pa += G_FORCE;
        }
    }
}

void check_boundary(Grid& grid) {
    for (int ci = 0; ci < grid.pos_x.size(); ci++) {
        for (int pi = 0; pi < grid.pos_x[ci].size(); pi++) {
            if (grid.pos_x[ci][pi] < 0)
                grid.pos_x[ci][pi] += (0 - grid.pos_x[ci][pi]);
            else if (grid.pos_x[ci][pi] + PART_R * 2 > WIN_W) 
                grid.pos_x[ci][pi] -= (grid.pos_x[ci][pi] + PART_R * 2 - WIN_W);
            if (grid.pos_y[ci][pi] + PART_R * 2 > WIN_H)
                grid.pos_y[ci][pi] -= (grid.pos_y[ci][pi] + PART_R * 2 - WIN_H);
        }
    }
}

void check_cell_collision(Grid& grid, int curr_cell, int other_cell) {
    for (int pi = 0; pi < grid.pos_x[curr_cell].size(); pi++) {
        for (int opi = 0; opi < grid.pos_x[other_cell].size(); opi++) {
            if (pi == opi && curr_cell == other_cell) {continue;}
            float x = grid.pos_x[curr_cell][pi];
            float y = grid.pos_y[curr_cell][pi];

            float ox = grid.pos_x[other_cell][opi];
            float oy = grid.pos_y[other_cell][opi];

            float diff_x = x - ox;
            float diff_y = y - oy;
            float dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);

            if (dist < PART_R * 2) {
                float norm_x = diff_x / dist;
                float norm_y = diff_y / dist;
                float delta = PART_R * 2 - dist;

                std::lock_guard<std::mutex> lock(mut);
                grid.pos_x[other_cell][opi] -= 0.5f * delta * norm_x;
                grid.pos_y[other_cell][opi] -= 0.5f * delta * norm_y;
                grid.pos_x[curr_cell][pi]   += 0.5f * delta * norm_x;
                grid.pos_y[curr_cell][pi]   += 0.5f * delta * norm_y;
            }
        }
    }
}

void check_collision_for_seg(Grid& grid, int seg_start, int seg_end) {
    for (int ci = seg_start; ci < seg_end; ci++) {
        check_cell_collision(grid, ci, ci);

        if (ci + GRID_COLS < grid.pos_x.size()) 
            check_cell_collision(grid, ci, ci + GRID_COLS);
        if (ci - GRID_COLS > 0) 
            check_cell_collision(grid, ci, ci - GRID_COLS);

        if (GRID_COLS % ci != 0) {
            if (ci > 0) 
                check_cell_collision(grid, ci, ci - 1);
            if (ci - 1 - GRID_COLS > 0) 
                check_cell_collision(grid, ci, ci - 1 - GRID_COLS);
            if (ci - 1 + GRID_COLS < grid.pos_x.size()) 
                check_cell_collision(grid, ci, ci - 1 + GRID_COLS);
        }
    }
}

void check_collision(Grid& grid) {
    std::vector<std::thread> threads;

    int seg_size = grid.pos_x.size() / N_THREADS;
    for (int i = 0; i < N_THREADS; i++) {
        int seg_start = i * seg_size;
        int seg_end = i == N_THREADS - 1 ? grid.pos_x.size() : seg_start + seg_size;
        threads.emplace_back(check_collision_for_seg, std::ref(grid), seg_start, seg_end);
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread& t){t.join();});
}

void update_pos(Grid& grid) {
    float dt = 1.0f / N_TS;
    sf::Vector2f vel;
    for (int ci = 0; ci < grid.pos_x.size(); ci++) {
        for (int pi = 0; pi < grid.pos_x[ci].size(); pi++) {
            vel = {grid.pos_x[ci][pi] - grid.prev_x[ci][pi], grid.pos_y[ci][pi] - grid.prev_y[ci][pi]};

            grid.prev_x[ci][pi] = grid.pos_x[ci][pi];
            grid.prev_y[ci][pi] = grid.pos_y[ci][pi];

            grid.pos_x[ci][pi] += (vel.x + grid.accel_x[ci][pi] * dt * dt);
            grid.pos_y[ci][pi] += (vel.y + grid.accel_y[ci][pi] * dt * dt);

            grid.accel_x[ci][pi] = 0;
            grid.accel_y[ci][pi] = 0;

            int new_grid_ind = grid_index(grid.pos_x[ci][pi], grid.pos_y[ci][pi]);
            if (new_grid_ind != ci && new_grid_ind < grid.pos_x.size()) {
                move_particle(grid, ci, new_grid_ind, pi);
            }
        }
    }
}


void render(sf::RenderWindow& window, Grid& grid) {
    sf::CircleShape foo(PART_R);
    for (int ci = 0; ci < grid.pos_x.size(); ci++) {
        for (int pi = 0; pi < grid.pos_x[ci].size(); pi++) {
            foo.setPosition({grid.pos_x[ci][pi], grid.pos_y[ci][pi]});
            window.draw(foo);
        }
    }
}

void epoch_grid(Grid& p, sf::RenderWindow& window) {
    for (int i = 0; i < N_TS; i++) {
        apply_grav(p);
        check_boundary(p);
        check_collision(p);
        update_pos(p);
        render(window, p);
    }
}
