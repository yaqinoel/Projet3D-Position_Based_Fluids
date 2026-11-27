#include "kernelFunctions.hpp"
#include <glm/geometric.hpp>

// poly6 kernel function to estimate density
float poly6Kernel(glm::vec3 r, float h) {
    float rSquared = glm::dot(r, r);
    float hSquared = h * h;

    // if distance > kernel radius, contribution = 0
    if (rSquared > hSquared) {
        return 0.0f;;
    }

    float h4Pow = hSquared * hSquared;
    float h9Pow = h4Pow * h4Pow * h;

    float diff = hSquared - rSquared;
    float diff3Pow = diff * diff * diff;
    return (coeffPoly6 / h9Pow) * diff3Pow;
}

// spikey kernel function to estimate density
float spikeyKernel(glm::vec3 r, float h) {
    float rLength = glm::length(r);

    // if distance > kernel radius, contribution = 0
    if (rLength > h) {
        return 0.0f;
    }

    float hSquared = h * h;
    float h6Pow = hSquared * hSquared * hSquared;

    float diff = h - rLength;
    float diff3Pow = diff * diff * diff;

    return (coeffSpikey / h6Pow) * diff3Pow;
}

// gradient of poly6 kernel function
glm::vec3 poly6KernelGradient(glm::vec3 r, float h) {
    float rSquared = glm::dot(r, r);
    float hSquared = h * h;

    if (rSquared > hSquared) {
        return glm::vec3(0.0f);
    }

    float h4Pow = hSquared * hSquared;
    float h9Pow = h4Pow * h4Pow * h;
    float diff = hSquared - rSquared;
    float diff2Pow = diff * diff;

    return (coeffPoly6Gradient / h9Pow) * diff2Pow * r;
}

// gradient of spikey kernel function
glm::vec3 spikeyKernelGradient(glm::vec3 r, float h) {
    float rLength = glm::length(r);

    if (rLength == 0.0f || rLength >= h) {
        return glm::vec3(0.0f);
    }

    float hSquared = h * h;
    float h6Pow = hSquared * hSquared * hSquared;

    float diff = h - rLength;
    float diff2Pow = diff * diff;

    return (coeffSpikeyGradient / h6Pow) * diff2Pow * r / rLength;
}