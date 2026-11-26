#pragma once

#include "Fluids.hpp"
#include "BoxContainer.hpp"

struct SimulationScene {
    Fluids fluids;
    BoxContainer container;
};