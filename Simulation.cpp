class Simulation {
    std::vector<Particle> particles;
    int max_particles;
    int n_substeps;

public:
    int num_particles;

    Simulation() {
        max_particles = 500;
        n_substeps = 2;
        num_particles = 0;
    }

    void add_particle() {
        particles.push_back(Particle());
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

    void update_positions() {
        const float sub_dt = 1.0f / static_cast<float>(n_substeps);
        for(int s = 0; s < n_substeps; s++) {
            for(int i = 0; i < particles.size(); i++) {
                particles[i].apply_grav();
                particles[i].check_boundary();
                for(int j = 0; j < particles.size(); j++) {
                    if(i != j) {
                        check_collision(particles[i], particles[j]);
                    }
                }
                particles[i].update_pos(sub_dt);
            }
        }
    }

    void render(sf::RenderWindow& window) {
        std::for_each(particles.begin(),
                      particles.end(),
                      [&window](Particle& pa) {
                          pa.render(window);
                      });
    }
};
