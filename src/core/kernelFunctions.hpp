#pragma once
#include "glm/vec3.hpp"
#include "glm/ext/scalar_constants.hpp"

constexpr float PI = glm::pi<float>();
constexpr float coeffPoly6 = 315.0f / (64.0f * PI);
constexpr float coeffPoly6Gradient = -945.0f / (32.0f * PI);
constexpr float coeffSpikey = 15.0f / PI;
constexpr float coeffSpikeyGradient = -45.0f / PI;


float poly6Kernel(glm::vec3 r, float h);

float spikeyKernel(glm::vec3 r, float h);

glm::vec3 poly6KernelGradient(glm::vec3 r, float h);

glm::vec3 spikeyKernelGradient(glm::vec3 r, float h);
