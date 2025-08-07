#include <map>

struct Cell {
    std::map<long, Particle> particles;

    void add(Particle& p) {
        if (particles.count(p.id) == 0) {
            particles.insert({p.id, p});
        }
    }

    void remove(int id) {
        particles.erase(id);
    }
};

class Simulation {
    std::array<std::array<Cell, WIN_W / CELL_W>, WIN_H / CELL_W> particles;
    std::map<long, Particle> oob_particles;
    int max_particles;
    int n_substeps;

public:
    long num_particles;

    Simulation() {
        max_particles = 500;
        n_substeps = 2;
        num_particles = 0;
    }

    void add_particle() {
        Particle p(num_particles);
        particles[p.grid_pos.y][p.grid_pos.x].add(p);
        num_particles++;
    }

    void check_collision(Particle& p1, Particle& p2) {
        sf::Vector2f diff = p1.pos - p2.pos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if(dist < p1.radius + p2.radius) {
            sf::Vector2f norm = diff / dist;
            float delta = p1.radius + p2.radius - dist;
            p1.pos += 0.5f * delta * norm;
            p2.pos -= 0.5f * delta * norm;

        }
    }

    void update_cell(Cell& c, int i, int j) {
        for(auto it = c.particles.begin(); it != c.particles.end(); ) {
            if(it->second.is_updated) {
                it++;
                continue;
            }
            it->second.apply_grav();
            it->second.check_boundary();
            for(auto it2 = c.particles.begin(); it2 != c.particles.end(); it2++) {
                if(it->second.id != it2->second.id) {
                    check_collision(it->second, it2->second);
                }
            }
            if(j < WIN_W / CELL_W - 1) {
                Cell adj = particles[i][j + 1];
                for(auto it2 = adj.particles.begin(); it2 != adj.particles.end(); it2++) {
                    if(it->second.id != it2->second.id) {
                        check_collision(it->second, it2->second);
                    }
                }
            }
            if(j > 0) {
                Cell adj = particles[i][j - 1];
                for(auto it2 = adj.particles.begin(); it2 != adj.particles.end(); it2++) {
                    if(it->second.id != it2->second.id) {
                        check_collision(it->second, it2->second);
                    }
                }
            }
            if(i > 0) {
                Cell adj = particles[i - 1][j];
                for(auto it2 = adj.particles.begin(); it2 != adj.particles.end(); it2++) {
                    if(it->second.id != it2->second.id) {
                        check_collision(it->second, it2->second);
                    }
                }
            }
            if(i < WIN_H / CELL_W - 1) {
                Cell adj = particles[i + 1][j];
                for(auto it2 = adj.particles.begin(); it2 != adj.particles.end(); it2++) {
                    if(it->second.id != it2->second.id) {
                        check_collision(it->second, it2->second);
                        it2->second.update_pos(1.0);
                    }
                }
            }
            it->second.update_pos(1.0);
            if(it->second.grid_pos != it->second.prev_grid_pos) {
                if(it->second.pos.y >= 0
                   && it->second.pos.y < WIN_H
                   && it->second.pos.x < WIN_W
                   && it->second.pos.x >= 0) {
                    particles[it->second.grid_pos.y][it->second.grid_pos.x].add(it->second);
                } else {
                    oob_particles.insert({it->second.id, it->second});
                }
                it = c.particles.erase(it);
            } else {
                it++;
            }
        }
    }

    void update_map() {
        for(auto it = oob_particles.begin(); it != oob_particles.end(); ) {
            it->second.apply_grav();
            it->second.check_boundary();
            it->second.update_pos(1.0);
            if(it->second.pos.y >= 0
               && it->second.pos.y < WIN_H
               && it->second.pos.x < WIN_W
               && it->second.pos.x >= 0) {
                particles[it->second.grid_pos.y][it->second.grid_pos.x].add(it->second);
                it = oob_particles.erase(it);
            } else {
                it++;
            }
        }
    }

    void update_positions() {
        const float sub_dt = 1.0f / static_cast<float>(n_substeps);
        for(int s = 0; s < sub_dt; s++) {
            for(int i = 0; i < particles.size(); i++) {
                for(int j = 0; j < particles[i].size(); j++) {
                    update_cell(particles[i][j], i, j);
                }
            }
            update_map();
        }
    }

    void render(sf::RenderWindow& window) {
        for(auto& row : particles) {
            for(auto& cell : row) {
                for(auto& pair : cell.particles) {
                    pair.second.render(window);
                    pair.second.is_updated = false;
                }
            }
        }
    }
};
