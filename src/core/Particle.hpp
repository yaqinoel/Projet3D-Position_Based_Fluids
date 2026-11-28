#pragma once

#include <glm/vec3.hpp>

struct Particle {
    float mass = 0.0f;
    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    float initialHeight = 0.0f;
};