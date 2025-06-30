
#include "kamikaze.hpp"
#include "../../sq.hpp"
#include "../../Game.h"

Kamikaze::Kamikaze(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, KAMIKAZE_RADIUS),
    m_should_delete(false) {} 

bool Kamikaze::target_inside_radius(const std::reference_wrapper<KinematicBody> targets[], const size_t amount_targets) const {
    for(size_t i = 0; i < amount_targets; i++) {
        auto &possible_target = targets[i].get();
        const auto dist = m_position - possible_target.m_position;
        const auto dist_sq = sq(dist.x) + sq(dist.y);
        if(dist_sq < sq(KAMIKAZE_DETONATION_RADIUS)) {
            return true;
        }
    }
    return false;
}

void Kamikaze::draw(Game &game) const {
    game.draw_ellipsis(m_position, {KAMIKAZE_RADIUS * 2, KAMIKAZE_RADIUS * 2});
}

void Kamikaze::on_collision() {
    m_should_delete = true;
}