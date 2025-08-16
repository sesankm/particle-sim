constexpr int   N_TS = 1.0;
constexpr int   MAX_PARTICLES = 1;
constexpr float G_FORCE = 0.098;


struct Data {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> px;
    std::vector<float> py;
    std::vector<float> accel_x;
    std::vector<float> accel_y;
};

void add_particle(Data& data) {
    data.x.push_back(rand() % 150 + 50);
    data.y.push_back(200);

    data.px.push_back(data.x.back());
    data.py.push_back(200);

    data.accel_x.push_back(0);
    data.accel_y.push_back(0);


    sf::CircleShape shape(PART_R);
    shape.setFillColor(sf::Color(255, 255, 255));
}

void apply_grav(Data& data) {
    std::for_each(data.accel_y.begin(), data.accel_y.end(), [](float& ay) {
        ay += G_FORCE;
    });
}

void check_boundary(Data& data) {
    for (int i = 0; i < data.y.size(); i++) {
        if(data.y[i] + PART_R * 2 > WIN_H)
            data.y[i] -= (data.y[i] + PART_R * 2 - WIN_H);
        if(data.x[i] < 0)
            data.x[i] += (0 - data.x[i]);
        else if(data.x[i] + PART_R * 2 > WIN_W)
            data.x[i] -= (data.x[i] + PART_R * 2 - WIN_W);
    }
}

void check_collision(Data& data) {
    for (int i = 0; i < data.x.size(); i++) {
        for (int j = 0; j < data.x.size(); j++) {
            if (i != j
                && std::abs(data.x[j] - data.x[i]) < PART_R * 2
                && std::abs(data.y[j] - data.y[i]) < PART_R * 2) {
                float diff_x = data.x[i] - data.x[j];
                float diff_y = data.y[i] - data.y[j];
                float dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);

                if (dist < PART_R * 2) {
                    float norm_x = diff_x / dist;
                    float norm_y = diff_y / dist;
                    float delta = PART_R * 2 - dist;

                    data.x[i] += 0.5f * delta * norm_x;
                    data.y[i] += 0.5f * delta * norm_y;
                    data.x[j] -= 0.5f * delta * norm_x;
                    data.y[j] -= 0.5f * delta * norm_y;
                }
            }
        }
    }
}

void update_pos(Data& data) {
    float dt = 1.0f / N_TS;
    sf::Vector2f vel;
    for (int i = 0; i < data.x.size(); i++) {
        vel = {data.x[i] - data.px[i], data.y[i] - data.py[i]};
        data.px[i] = data.x[i];
        data.py[i] = data.y[i];
        data.x[i] += (vel.x + data.accel_x[i] * dt * dt);
        data.y[i] += (vel.y + data.accel_y[i] * dt * dt);

        data.accel_x[i] = 0;
        data.accel_y[i] = 0;
    }
}

void render(sf::RenderWindow& window, Data& data) {
    sf::CircleShape foo(PART_R);
    for (int i = 0; i < data.x.size(); i++) {
        foo.setPosition({data.x[i], data.y[i]});
        window.draw(foo);
    }
}

void epoch(Data& p, int num_particles, sf::RenderWindow& window) {
    for (int i = 0; i < N_TS; i++) {
        apply_grav(p);
        check_boundary(p);
        check_collision(p);
        update_pos(p);
        render(window, p);
    }
}
