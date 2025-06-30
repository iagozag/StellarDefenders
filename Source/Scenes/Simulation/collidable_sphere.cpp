
#include "collidable_sphere.hpp"

CollidableSphere::CollidableSphere(const float radius):
    m_radius(radius) {}

bool CollidableSphere::collides(const CollidableSphere &other) const {
    const auto cur_pos = get_position();
    const auto other_pos = other.get_position();
    const auto distance = glm::distance(cur_pos, other_pos);
	const auto sum_of_radii = m_radius + other.m_radius;
	return distance <= sum_of_radii;
}

float CollidableSphere::get_radius() const {
    return m_radius;
}
