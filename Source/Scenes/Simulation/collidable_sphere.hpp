
#pragma once

#include <glm/glm.hpp>
#include <functional>

class CollidableSphere {
public:
    CollidableSphere(const float radius);
    virtual ~CollidableSphere() = default;

    virtual glm::vec2 get_position() const = 0;
    virtual void on_collision() = 0;
    
    bool collides(const CollidableSphere &other) const;
    float get_radius() const;

protected:
    float m_radius;
};
