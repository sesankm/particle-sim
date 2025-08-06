struct Cell {
    std::vector<Particle> particles;

    void remove_particle(Particle& p) {
        particles.erase(std::remove_if(particles.begin(),
                                       particles.end(),
                                       [p](Particle& o) {
                                           return p.prev_pos == o.prev_pos
                                               && p.pos == o.pos
                                               && p.grid_pos == o.grid_pos
                                               && p.prev_grid_pos == o.prev_grid_pos;
                                       }), particles.end());
    }

    void push_back(Particle& p) {
        particles.push_back(p);
    }
};

struct Grid {
    std::array<std::array<Cell, WIN_W / 10>, WIN_H / 10> cells;
    std::vector<Cell> out_of_bounds;

    void add_particle() {
        Particle p;
        cells[p.grid_pos.y][p.grid_pos.x].push_back(p);
    }

    void add_particle(Particle& p) {
        cells[p.grid_pos.y][p.grid_pos.x].push_back(p);
    }

    void remove_particle(Particle& p) {
        cells[p.grid_pos.y][p.grid_pos.x].remove_particle(p);
    }
};

