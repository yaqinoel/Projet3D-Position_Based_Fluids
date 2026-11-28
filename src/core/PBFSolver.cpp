#include <iostream>

#include "PBFSolver.hpp"
#include "kernelFunctions.hpp"

#include <glm/common.hpp>
#include <glm/geometric.hpp>


// initialize solver with particles
PBFSolver::PBFSolver(const float density, const std::vector<Particle>& initialParticles, BoxContainer box, SimulationParams params)
    : restDensity(density), particles(initialParticles), container(box), params(params), searchEngine(params.kernelRadius) {
    std::cout<< "Initialized solver with " << particles.size() << " particles" << std::endl;
}


// execute simulation one step
void PBFSolver::step(float dt) {
    if (dt <= 0.0f) {
        throw std::runtime_error("PBFSolver::step: dt is not positive");
    }

    // clean history
    predictPositions.resize(particles.size());
    lamda.resize(particles.size());
    density.resize(particles.size());
    constraintDensity.resize(particles.size());
    vorticity.assign(particles.size(), glm::vec3(0.0f));
    xsphVelocityCorrection.assign(particles.size(), glm::vec3(0.0f));

    const glm::vec3 boxMin = container.center - container.halfSize;
    const glm::vec3 boxMax = container.center + container.halfSize;

    std::size_t iteration = 0;

    // for all particles predict new positions
    for (int i = 0; i < particles.size(); i++) {
        // apply forces v_i <= v_i + dt * f(x_i)
        auto& p = particles[i];
        p.velocity = p.velocity + dt * params.gravity;
        // predict position
        predictPositions[i] = p.position + dt * p.velocity;
    }

    // find neighboring particles
    searchEngine.runSearch(predictPositions);

    deltaPositions.resize(particles.size());
    const float restDensityInv = 1.0f / restDensity;
    const float h = params.kernelRadius;

    while (iteration < params.numInteration) {
        // estimate density and solve each density constraint.
        for (std::size_t i = 0; i < particles.size(); ++i) {
            const glm::vec3& position = predictPositions[i];
            const auto& neighbors = searchEngine.getNeighbors(i);
            // the neighbor search excludes the particle itself.
            float particleDensity = particles[i].mass * poly6Kernel(glm::vec3(0.0f), h);
            float squaredGradientSum = 0.0f;
            glm::vec3 centerGradient(0.0f);

            for (const std::size_t j : neighbors) {
                const glm::vec3 offset = position - predictPositions[j];
                const float mass = particles[j].mass;
                particleDensity += mass * poly6Kernel(offset, h);

                const glm::vec3 gradient = (mass * restDensityInv) * spikeyKernelGradient(offset, h);
                centerGradient += gradient;
                squaredGradientSum += glm::dot(gradient, gradient);
            }

            density[i] = particleDensity;
            constraintDensity[i] = particleDensity * restDensityInv - 1.0f;
            // all fluid particles have the same mass.
            const float gradientSquaredSum = squaredGradientSum + glm::dot(centerGradient, centerGradient);
            const float denominator = gradientSquaredSum / particles[i].mass + params.epsilon_cfm;
            lamda[i] = -constraintDensity[i] / denominator;
        }

        // compute all corrections before updating any predicted position.
        for (std::size_t i = 0; i < particles.size(); ++i) {
            const glm::vec3& position = predictPositions[i];
            const auto& neighbors = searchEngine.getNeighbors(i);
            glm::vec3 correction(0.0f);
            for (const std::size_t j : neighbors) {
                // add an artificial pressure
                const glm::vec3& offset = position - predictPositions[j];
                const glm::vec3 delta_q = params.coeff_delta_q * params.kernelRadius * glm::vec3(0.0f, 1.0f, 0.0f);
                const float numerator = poly6Kernel(offset, params.kernelRadius);
                const float denominator = poly6Kernel(delta_q, params.kernelRadius);

                float s_corr = 0.0f;
                if (params.useArtificialPressure) {
                    s_corr = - params.s_corr_k * std::pow(( numerator / denominator), params.s_corr_n);
                }

                const float scale = lamda[i] + lamda[j] +s_corr;
                correction += scale * spikeyKernelGradient(position - predictPositions[j], h);
            }
            deltaPositions[i] = restDensityInv * correction;
        }

        // apply the position and wall corrections together.
        for (std::size_t i = 0; i < particles.size(); ++i) {
            predictPositions[i] = glm::clamp(predictPositions[i] + deltaPositions[i], boxMin, boxMax);
        }
        ++iteration;
    }

    // update velocity
    for (int i = 0; i < particles.size(); i++) {
        auto& p = particles[i];
        p.velocity = params.damping * (predictPositions[i] - p.position) / dt;
    }

    // vorticity confinement
    if (params.useVortivityConfinement) {
        // update vorticity
        for (int i = 0; i < particles.size(); i++) {
            auto& p = particles[i];
            // calculate vorticity
            const auto& neighbors = searchEngine.getNeighbors(i);
            for (const std::size_t j : neighbors) {
                const glm::vec3 offset = predictPositions[i] - predictPositions[j];
                glm::vec3 velocityDiff = particles[j].velocity - particles[i].velocity;
                glm::vec3 gradient = - (p.mass * restDensityInv) * spikeyKernelGradient(offset, params.kernelRadius);
                vorticity[i] += glm::cross(velocityDiff, gradient);
            }
        }

        // update vorticity gradient and force
        for (int i = 0; i < particles.size(); i++) {
            auto& p = particles[i];
            glm::vec3 vorticityGradient(0.0f);
            // calculate vorticity
            const auto& neighbors = searchEngine.getNeighbors(i);
            for (const std::size_t j : neighbors) {
                const glm::vec3 offset = predictPositions[i] - predictPositions[j];
                float vorticityDiff = glm::length(vorticity[j]) - glm::length(vorticity[i]);
                glm::vec3 gradient = (p.mass * restDensityInv) * spikeyKernelGradient(offset, params.kernelRadius);
                float coeff = particles[j].mass / density[j];
                vorticityGradient += coeff * vorticityDiff * gradient;
            }

            if (glm::dot(vorticityGradient, vorticityGradient) > 1e-9) {
                vorticityGradient = glm::normalize(vorticityGradient);
                glm::vec3 forceVelocity = glm::cross(vorticityGradient, vorticity[i]);
                p.velocity += forceVelocity / p.mass * dt;
            }
        }
    }

    // XSPH
    if (params.useXSPH) {
        for (int i = 0; i < particles.size(); i++) {
            const auto& neighbors = searchEngine.getNeighbors(i);
            for (const std::size_t j : neighbors) {
                const glm::vec3 offset = predictPositions[i] - predictPositions[j];
                glm::vec3 velocityDiff = particles[j].velocity - particles[i].velocity;
                xsphVelocityCorrection[i] += velocityDiff * poly6Kernel(offset, params.kernelRadius);
            }
        }
        for (int i = 0; i < particles.size(); i++) {
            auto& p = particles[i];
            p.velocity += params.xsph_c * xsphVelocityCorrection[i];
        }
    }

    // update position
    for (int i = 0; i < particles.size(); i++) {
        auto& p = particles[i];
        p.position = predictPositions[i];
    }

}


// set new particles for solver
void PBFSolver::setParticles(const std::vector<Particle>& newParticles) {
    std::cout<< "Reset solver with " << particles.size() << " particles" << std::endl;
    particles = newParticles;
}


// get all particles, just for read, only the solver can change particles attributions
[[nodiscard]]
const std::vector<Particle>& PBFSolver::getParticles() const {
    return particles;
}
