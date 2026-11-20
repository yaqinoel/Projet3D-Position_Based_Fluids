#pragma once

#include <glad/gl.h>
#include <vector>

#include "Particle.hpp"

class ParticleRender {
public:
    ParticleRender();
    ~ParticleRender();

    ParticleRender(const ParticleRender&) = delete;
    ParticleRender& operator=(const ParticleRender&) = delete;

    void upload(const std::vector<Particle>& particles);    // upload particles to gpu
    void draw() const;  // draw particles in openGL

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei particleCount = 0;
};