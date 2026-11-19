#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtc/random.hpp>

#include "PBFSolver.hpp"
#include "core/Particle.hpp"

int main() {
    std::vector<Particle> particles;

    constexpr int num_particles = 10000;

    // initialize particles
    particles.resize(num_particles);
    for (int i = 0; i < num_particles; i++) {
        particles[i].mass = 1.0f;
        particles[i].velocity = glm::vec3(0.0f);
        const glm::vec3 random = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
        const glm::vec3 halfSize{0.5f, 1.5f, 0.5f};
        const glm::vec3 center{-0.5f, 2.5f, 0.0f};
        // set all particles randomly in a cube
        particles[i].position = center + halfSize * random;
    }

    // create solver with particles
    PBFSolver solver(particles);



}