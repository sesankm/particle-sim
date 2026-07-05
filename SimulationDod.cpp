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
    int ind = grid_index(x, y);

    grid.pos_x.push_back(x);
    grid.pos_y.push_back(y);

    grid.prev_x.push_back(x);
    grid.prev_y.push_back(y);

    grid.accel_x.push_back(0);
    grid.accel_y.push_back(0);
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

void check_collision(Grid& grid) {
    for (int pi = 0; pi < grid.pos_x.size(); pi++) {
        for (int opi = 0; opi < grid.pos_x.size(); opi++) {
            if (pi == opi) {continue;}
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
        update_pos(p);
        render(window, p);
    }
}
