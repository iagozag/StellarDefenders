
#pragma once

#include "particle.hpp"
#include "fragment.hpp"

class Game;

constexpr float KAMIKAZE_DETONATION_RADIUS = 0.08;
constexpr float KAMIKAZE_RADIUS = 0.01;

enum class ShipState
{
    Positioning,
    SlingShot,
    Ready,
    Simulating
};

class Kamikaze: public Particle {
public:
    Kamikaze(const glm::vec2 &position, const glm::vec2 &speed);

    bool target_inside_radius(const std::reference_wrapper<KinematicBody> targets[], const size_t amount_targets) const;
    void draw(Game &game) const;

    std::vector<Fragment> generate_fragments() const;

    void on_collision() override;

    bool m_should_delete: 1;

    void OnProcessInput(const uint8_t* state);

    void OnUpdate(Game&, float);

    void DrawSlingShotLine(Game&, std::vector<glm::vec2>) const;

    glm::vec2 get_direction() const{ return mDirection; }

    ShipState GetShipState() const{ return mShipState; }
    void SetShipState(ShipState shipState) { mShipState = shipState; }

private:
    ShipState mShipState;

    glm::vec2 mOriginPos;
    glm::vec2 mSlingShotMousePos;
    glm::vec2 mDirection;
};
