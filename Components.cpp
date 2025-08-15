constexpr int   MAX_PARTICLES = 1;
constexpr int   N_TS = 1.0;
constexpr float G_FORCE = 0.098;


struct Position {
    std::vector<float> x;
    std::vector<float> y;

    std::vector<float> px;
    std::vector<float> py;

    std::vector<float> accel_x;
    std::vector<float> accel_y;
};


struct Figure {
    std::vector<sf::CircleShape> figures;
};

void add_particle(Position& positions, Figure& fig) {
    positions.x.push_back(rand() % 150 + 50);
    positions.y.push_back(200);

    positions.px.push_back(positions.x.back());
    positions.py.push_back(200);

    positions.accel_x.push_back(0);
    positions.accel_y.push_back(0);

    fig.figures.push_back(sf::CircleShape(PART_R));
}

void apply_grav(Position& p) {
    std::for_each(p.accel_y.begin(), p.accel_y.end(), [](float& ay) {
        ay += G_FORCE;
    });
}

void check_boundary(Position& pos) {
    for (int i = 0; i < pos.y.size(); i++) {
        if(pos.y[i] + PART_R * 2 > WIN_H)
            pos.y[i] -= (pos.y[i] + PART_R * 2 - WIN_H);
        if(pos.x[i] < 0)
            pos.x[i] += (0 - pos.x[i]);
        else if(pos.x[i] + PART_R * 2 > WIN_W)
            pos.x[i] -= (pos.x[i] + PART_R * 2 - WIN_W);
    }
}

void check_collision(Position& pos) {
    for (int i = 0; i < pos.x.size(); i++) {
        for (int j = 0; j < pos.x.size(); j++) {
            if (i != j) {
                float diff_x = pos.x[i] - pos.x[j];
                float diff_y = pos.y[i] - pos.y[j];
                float dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);

                if (dist < PART_R * 2) {
                    float norm_x = diff_x / dist;
                    float norm_y = diff_y / dist;
                    float delta = PART_R * 2 - dist;

                    pos.x[i] += 0.5f * delta * norm_x;
                    pos.y[i] += 0.5f * delta * norm_y;
                    pos.x[j] -= 0.5f * delta * norm_x;
                    pos.y[j] -= 0.5f * delta * norm_y;
                }
            }
        }
    }
}

void update_pos(Position& p) {
    float dt = 1.0f / N_TS;
    sf::Vector2f vel;
    for (int i = 0; i < p.x.size(); i++) {
        vel = {p.x[i] - p.px[i], p.y[i] - p.py[i]};
        p.px[i] = p.x[i];
        p.py[i] = p.y[i];
        p.x[i] += (vel.x + p.accel_x[i] * dt * dt);
        p.y[i] += (vel.y + p.accel_y[i] * dt * dt);

        p.accel_x[i] = 0;
        p.accel_y[i] = 0;
    }
}


void render(sf::RenderWindow& window, Figure& f, Position& pos) {
    for (int i = 0; i < pos.x.size(); i++) {
        f.figures[i].setPosition({pos.x[i], pos.y[i]});
        f.figures[i].setFillColor(sf::Color(255, 255, 255));
        window.draw(f.figures[i]);
    }
}

void epoch(Figure& f, Position& p, int num_particles, sf::RenderWindow& window) {
    for (int i = 0; i < N_TS; i++) {
        apply_grav(p);
        check_boundary(p);
        check_collision(p);
        update_pos(p);
        render(window, f, p);
    }
}
