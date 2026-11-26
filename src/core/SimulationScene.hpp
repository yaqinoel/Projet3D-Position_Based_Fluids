#pragma once

#include <glm/vec3.hpp>

#include "Fluids.hpp"

struct BoxContainer {
    glm::vec3 center;
    glm::vec3 halfSize;
};

struct SimulationScene {
    Fluids fluids;
    BoxContainer container;
};