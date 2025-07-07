
#include "simulation.hpp"
#include "../../Game.h"
#include "../../sq.hpp"
#include "../../filter_vector.hpp"

#include <iostream>
#include <unordered_set>
#include <limits>

Simulation::Simulation(
    std::vector<Planet> planets,
    std::vector<Target> targets,
    const float duration,
    const uint32_t ships_to_be_positioned,
    const uint32_t level,
    UIScreen *screen
):
    m_planets(std::move(planets)),
    m_targets(std::move(targets)),
    m_time_simulated(0),
    m_duration(duration),
    m_screen(screen),
    m_backup(nullptr),
    m_ships_to_be_positioned(ships_to_be_positioned),
    m_level(level),
    m_locked(true),
    m_running(false)
{
    if(m_screen) {
        m_screen->AddButton("Stop", glm::vec2(0.45, -0.55), glm::vec2(0.5, 0.2), [this]() {
            if(!m_locked) {
                *this = std::move(*m_backup);
                delete m_backup;
                m_backup = nullptr;
            }
        });
        m_screen->AddButton("Simulate", glm::vec2(0.45, -0.75), glm::vec2(0.5, 0.2), [this]() {
            if(m_locked) {
                m_backup = new Simulation(*this);
                unlock();
                m_running = true;
            }
        });
        m_screen->AddButton("Remove all", glm::vec2(0.45, -0.95), glm::vec2(0.5, 0.2), [this]() {
            if(!m_running) {
                m_ships_to_be_positioned += m_kamikaze.size();
                m_kamikaze.clear();
            }
        });
    }
}

bool Simulation::all_enemies_dead() const {
    return (int)m_targets.size() == 0;
}

Simulation::~Simulation() {
    delete m_backup;
}

void Simulation::draw(Game &game) const {
    for(auto &planet : m_planets) {
        planet.draw(game, !m_running);
    }

    for(auto &kamikaze : m_kamikaze) {
        kamikaze.draw(game);
    }

    for(auto &target : m_targets) {
        target.draw(game);
    }

    for(auto &fragment : m_fragments) {
        fragment.draw(game);
    }

    if(m_locked && !m_running && m_time_simulated == 0 && m_ships_to_be_positioned) {
        m_virtual_positioning.draw(game, *this);
    }
}

void Simulation::process_input(Game &game, const uint8_t* state) {
    if(m_locked && !m_running && m_time_simulated == 0 && m_ships_to_be_positioned) {
        m_virtual_positioning.process_input(game, *this);
    }
}

bool Simulation::is_locked() const {
    return m_locked;
}

const Planet *Simulation::get_nearest_positionable_planet(const glm::vec2 &position) const {
    const Planet *nearest = nullptr;
    for(auto &planet : m_planets) {
        if(planet.m_positioning_radius != 0) {
            if(!nearest) {
                nearest = &planet;
            } else {
                const auto dist_nearest = glm::length(nearest->m_position - position);
                const auto cur_dist = glm::length(planet.m_position - position);
                if(cur_dist < dist_nearest) {
                    nearest = &planet;   
                }
            }
        }
    }

    return nearest;
}

std::vector<glm::vec2> Simulation::simulate(Game &game, const glm::vec2 &position, const glm::vec2 &speed) const {
    auto copy = Simulation(m_planets, {}, 100, 1, 0, nullptr);
    copy.add_kamikaze(position, speed);
    copy.unlock();

    constexpr float DELTA_T = 1 / 60.f;
    constexpr float DURATION = 10;
    constexpr size_t NUM_STEPS = DURATION / DELTA_T;

    std::vector<glm::vec2> res;
    res.reserve(NUM_STEPS);
    for(size_t i = 0; i < NUM_STEPS && copy.m_kamikaze.size(); i++) {
        res.push_back(copy.m_kamikaze[0].m_position);
        copy.run(game, DELTA_T);
    }

    return res;
}

glm::vec2 calculate_acceleration(const glm::vec2 &body_pos, const Planet &attractor) {
    const auto diff = attractor.m_position - body_pos;
    const auto dist_sq = sq(diff.x) + sq(diff.y);
    const auto force_dir = glm::normalize(diff);
    return attractor.m_mass * force_dir / dist_sq;
}

void Simulation::run(Game &game, const float delta_t, const bool ignore_collision) {
    if(all_enemies_dead()){
        game.SetGameScene(Game::GameScene::Ship, Game::TRANSITION_TIME);
        game.SetCurrentLevel(m_level+1);
    }

    if(m_screen) {
        m_screen->clear_text();
        const auto rendered_text = std::to_string(m_ships_to_be_positioned) + " ship remaining";
        m_screen->AddText(rendered_text, glm::vec2(-0.925, -0.925), glm::vec2(1, 0.10));
    }

    if(m_locked) {
        return;
    }

    if(finished()) {
        lock();
        return;
    }

    glm::vec2 kamikaze_accelerations[m_kamikaze.size()];
    for(auto &acel : kamikaze_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }

    glm::vec2 planet_accelerations[m_planets.size()];
    for(auto &acel : planet_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }

    glm::vec2 target_accelerations[m_targets.size()];
    for(auto &acel : target_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }

    glm::vec2 fragment_accelerations[m_fragments.size()];
    for(auto &acel : fragment_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }


    for(size_t i = 0; i < m_planets.size(); i++) {
        for(size_t j = i + 1; j < m_planets.size(); j++) {
            const auto diff = m_planets[i].m_position - m_planets[j].m_position;
            const auto dist_sq = std::max(sq(diff.x) + sq(diff.y), 0.00001f);

            const auto force_dir = glm::normalize(diff);

            const glm::vec2 accel_on_i = m_planets[j].m_mass * force_dir / dist_sq;
            const glm::vec2 accel_on_j = m_planets[i].m_mass * force_dir / dist_sq;

            planet_accelerations[i] -= accel_on_i;
            planet_accelerations[j] += accel_on_j;
        }

        for(size_t j = 0; j < m_kamikaze.size(); j++) {
            kamikaze_accelerations[j] += calculate_acceleration(m_kamikaze[j].m_position, m_planets[i]);
        }

        for(size_t j = 0; j < m_targets.size(); j++) {
            target_accelerations[j] += calculate_acceleration(m_targets[j].m_position, m_planets[i]);
        }

        for(size_t j = 0; j < m_fragments.size(); j++) {
            fragment_accelerations[j] += calculate_acceleration(m_fragments[j].m_position, m_planets[i]);
        }
    }

    for(size_t i = 0; i < m_planets.size(); i++) {
        m_planets[i].apply_acceleration(planet_accelerations[i], delta_t);
    }

    for(size_t i = 0; i < m_kamikaze.size(); i++) {
        auto& k = m_kamikaze[i];
        k.apply_acceleration(kamikaze_accelerations[i], delta_t);
    }

    for(size_t i = 0; i < m_targets.size(); i++) {
        m_targets[i].apply_acceleration(target_accelerations[i], delta_t);
    }
    
    for(size_t i = 0; i < m_fragments.size(); i++) {
        m_fragments[i].apply_acceleration(fragment_accelerations[i], delta_t);
        m_fragments[i].m_time_alive += delta_t;
    }

    if(!ignore_collision) {
        run_collision_tests();
    }
}

static glm::vec2 v_min(const glm::vec2 &a, const glm::vec2 &b) {
    return glm::vec2(std::min(a.x, b.x), std::min(a.y, b.y));
}

static glm::vec2 v_max(const glm::vec2 &a, const glm::vec2 &b) {
    return glm::vec2(std::max(a.x, b.x), std::max(a.y, b.y));
}

struct PairHasher {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        return hash1 ^ (hash2 << 1);
    }
};

void Simulation::run_collision_tests() {
    constexpr size_t MAX_GRID_SIZE = 32;

    std::vector<std::reference_wrapper<CollidableSphere>> grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    glm::vec2 start(std::numeric_limits<float>::infinity()), end(-std::numeric_limits<float>::infinity());

    for(auto &planet : m_planets) {
        start = v_min(start, planet.m_position - planet.get_radius());
        end = v_max(end, planet.m_position + planet.get_radius());
    }
    for(auto &target : m_targets) {
        start = v_min(start, target.m_position - target.get_radius());
        end = v_max(end, target.m_position + target.get_radius());
    }
    for(auto &fragment : m_fragments) {
        start = v_min(start, fragment.m_position - fragment.get_radius());
        end = v_max(end, fragment.m_position + fragment.get_radius());
    }
    for(auto &kamikaze : m_kamikaze) {
        start = v_min(start, kamikaze.m_position - kamikaze.get_radius());
        end = v_max(end, kamikaze.m_position + kamikaze.get_radius());
    }

    const auto world_size = end - start;
    const auto inv_cell_size = float(MAX_GRID_SIZE) / world_size;
    auto add_to_grid = [start, inv_cell_size, &grid](Particle& body) {
        const glm::vec2 body_min = body.m_position - body.get_radius();
        const glm::vec2 body_max = body.m_position + body.get_radius();

        glm::ivec2 grid_min = glm::max(glm::ivec2(0), glm::ivec2((body_min - start) * inv_cell_size));
        glm::ivec2 grid_max = glm::min(glm::ivec2(MAX_GRID_SIZE - 1), glm::ivec2((body_max - start) * inv_cell_size));

        for(int y = grid_min.y; y <= grid_max.y; ++y) {
            for(int x = grid_min.x; x <= grid_max.x; ++x) {
                grid[y][x].push_back(body);
            }
        }
    };

    for(auto& planet : m_planets) {
        add_to_grid(planet);
    }
    for(auto& target : m_targets) {
        add_to_grid(target);
    }
    for(auto& fragment : m_fragments) {
        add_to_grid(fragment);
    }
    for(auto& kamikaze : m_kamikaze) {
        add_to_grid(kamikaze);
    }

    using SpherePair = std::pair<CollidableSphere*, CollidableSphere*>;
    std::unordered_set<SpherePair, PairHasher> tested_pairs;
    for(size_t y = 0; y < MAX_GRID_SIZE; ++y) {
        for(size_t x = 0; x < MAX_GRID_SIZE; ++x) {
            auto& cell = grid[y][x];
            if(cell.size() < 2) {
                continue;
            }

            for(size_t i = 0; i < cell.size(); ++i) {
                for(size_t j = i + 1; j < cell.size(); ++j) {
                    CollidableSphere& coli = cell[i].get();
                    CollidableSphere& colj = cell[j].get();

                    CollidableSphere* p1 = &coli;
                    CollidableSphere* p2 = &colj;
                    if (p1 > p2) std::swap(p1, p2);

                    if (tested_pairs.insert({p1, p2}).second) {
                        if (coli.collides(colj)) {
                            coli.on_collision();
                            colj.on_collision();
                        }
                    }
                }
            }
        }
    }


    std::vector<std::reference_wrapper<KinematicBody>> targets;
    targets.reserve(m_targets.size());
    for(size_t i = 0; i < m_targets.size(); i++) {
        targets.push_back(m_targets[i]);
    }

    for(size_t i = 0; i < m_kamikaze.size(); i++) {
        if(m_kamikaze[i].target_inside_radius(targets.data(), targets.size())) {
            m_kamikaze[i].m_should_delete = true;
            add_fragments(m_kamikaze[i].generate_fragments());
        }
    }


    delete_dead();
}

void Simulation::delete_dead() {
    const auto kamikaze_predicate = std::function([](const Kamikaze &kamikaze) {
        return !kamikaze.m_should_delete;
    });

    const auto target_predicate = std::function([](const Target &target) {
        return !target.m_should_delete;
    });

    const auto fragment_predicate = std::function([](const Fragment &fragment) {
        return !fragment.m_should_delete;
    });
    
    filter(m_kamikaze, kamikaze_predicate);
    filter(m_targets, target_predicate);
    filter(m_fragments, fragment_predicate);
}

void Simulation::add_fragments(const std::vector<Fragment> &fragments) {
    for(auto &fragment : fragments) {
        m_fragments.push_back(fragment);
    }
}

void Simulation::lock() {
    m_locked = true;
}

void Simulation::unlock() {
    m_locked = false;
}

void Simulation::add_kamikaze(const glm::vec2 &position, const glm::vec2 &speed) {
    m_ships_to_be_positioned--;
    m_kamikaze.emplace_back(position, speed);
}

bool Simulation::finished() const {
    return m_time_simulated >= m_duration;
}
