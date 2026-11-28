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
    bool useArtificialPressure = true;
    bool useVortivityConfinement = true;
    bool useXSPH = true;
    std::size_t numInteration = 4; // interation numbers in solver

    TimeStepMode timeStepMode = TimeStepMode::Fixed;    // step time mode
    ParticleSamplingMode particleSamplingMode = ParticleSamplingMode::BySpacing;    // sampling mode when initialize particles

    float particleSpacing = 0.0f;   // particle spacing when initialize particles
    float fixedTimeStep = 1.0f / 120.0f; // simulation fixed step time
    float containerWidthCycleDuration = 1.0f; // seconds per cycle, zero keeps the width fixed
    float kernelRadius = 0.3f;
    float epsilon_cfm = 25;
    float damping = 0.999f;
    float s_corr_k = 0.002f;
    float coeff_delta_q = 0.2f;
    float xsph_c = 1e-5;
    int s_corr_n = 4;

    std::size_t particleCount = 10000;  // particle numbers when initialize particles
    glm::vec3 gravity {0.0f, -9.81f, 0.0f };    // gravity
};
