#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>

#include "PBFSolver.hpp"
#include "render/Camera.hpp"
#include "render/Shader.hpp"
#include "render/ParticleRender.hpp"

// initialize GLFW and create window
GLFWwindow* createWindow() {
    glfwSetErrorCallback([](int, const char* message) {
        std::cerr << "GLFW: " << message << '\n';
    });

    if (!glfwInit()) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "PBF Viewer", nullptr, nullptr);

    if (!window) {
        return nullptr;
    }

    return window;
}


bool initializeOpenGL(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        std::cerr << "Failed to load OpenGL\n";
        return false;
    }

    glfwSwapInterval(1);
    glClearColor(0.06f, 0.09f, 0.14f, 1.0f);
    return true;
}

int main() {
    GLFWwindow* window = createWindow();

    if (window == nullptr) {
        return EXIT_FAILURE;
    }

    if (false == initializeOpenGL(window)) {
        return EXIT_FAILURE;
    }

    // create shader
    const auto shaderDir = std::filesystem::path{ASSET_DIR} / "shader";
    Shader shader{shaderDir / "particles.vert", shaderDir / "particles.frag"};
    ParticleRender renderer;

    // initialize particles
    Particle particle;
    particle.position = {0.0f, 0.0f, 0.0f};
    std::vector<Particle> particles;
    particles.push_back(particle);

    // initialize solver
    PBFSolver solver(particles);

    // upload particles to GPU
    renderer.upload(solver.getParticles());

    glEnable(GL_PROGRAM_POINT_SIZE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        if (width == 0 || height == 0) {
            glfwWaitEvents();
            continue;
        }

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // update particles positions
        solver.step(0.1f);
        renderer.upload(solver.getParticles());

        // draw particles
        shader.use();
        renderer.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}