#pragma once

#include <cstddef>
#include <glm/vec3.hpp>

enum class TimeStepMode {
    Fixed,
    FrameDelta
};

enum class ParticleSamplingMode {
    BySpacing,
    ByCount
};

// all simulation parameters
struct SimulationParams {
    std::size_t numInteration = 4; // interation numbers in solver

    TimeStepMode timeStepMode = TimeStepMode::Fixed;    // step time mode
    ParticleSamplingMode particleSamplingMode = ParticleSamplingMode::BySpacing;    // sampling mode when initialize particles

    float particleSpacing = 0.0f;   // particle spacing when initialize particles
    float fixedTimeStep = 1.0f / 60.0f; // simulation fixed step time
    float kernelRadius = 0.1f;

    std::size_t particleCount = 10000;  // particle numbers when initialize particles
    glm::vec3 gravity {0.0f, -9.81f, 0.0f };    // gravity
};
