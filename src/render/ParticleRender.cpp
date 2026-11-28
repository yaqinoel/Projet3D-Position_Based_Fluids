#include "ParticleRender.hpp"

// constructor for particle render
ParticleRender::ParticleRender() {
    // create VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// delete vao and vbo when descontruct render
ParticleRender::~ParticleRender() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}


// upload particles data to GPU
void ParticleRender::upload(const std::vector<Particle>& particles) {
    // collect positions of particles
    std::vector<float> particleData;
    particleData.reserve(particles.size() * 4);

    for (const Particle& particle : particles) {
        particleData.push_back(particle.position.x);
        particleData.push_back(particle.position.y);
        particleData.push_back(particle.position.z);
        particleData.push_back(particle.initialHeight);
    }

    const auto sizeInBytes = static_cast<GLsizeiptr>(particleData.size() * sizeof(float));

    // upload positions to vbo (gpu)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, particleData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    particleCount = static_cast<GLsizei>(particles.size());
}


// draw particles as points in openGL
void ParticleRender::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, particleCount);
    glBindVertexArray(0);
}