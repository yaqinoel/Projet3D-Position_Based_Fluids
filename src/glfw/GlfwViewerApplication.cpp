#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>

#include "PBFSolver.hpp"
#include "render/BoxContainerRender.hpp"
#include "render/Camera.hpp"
#include "render/Shader.hpp"
#include "render/ParticleRender.hpp"
#include "CameraInput.hpp"
#include "SimulationManager.hpp"
#include "SimulationInput.hpp"
#include "SimulationParams.hpp"

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


// create openGL context
bool initializeOpenGL(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        std::cerr << "Failed to load OpenGL\n";
        return false;
    }

    glfwSwapInterval(1);
    glClearColor(0.06f, 0.09f, 0.14f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    return true;
}


// create particles for test
std::vector<Particle> createParticles(const int num_particles = 100000) {
    std::vector<Particle> particles;
    particles.resize(num_particles);

    // set all particles randomly in a cube
    for (int i = 0; i < num_particles; i++) {
        particles[i].mass = 1.0f;
        particles[i].velocity = glm::vec3(0.0f);
        const glm::vec3 random = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
        const glm::vec3 halfSize{0.5f, 1.5f, 0.5f};
        const glm::vec3 center{0.0f, 2.5f, 0.0f};
        particles[i].position = center + halfSize * random;
    }

    return particles;
}

int main() {
    GLFWwindow* window = createWindow();

    if (window == nullptr) {
        return EXIT_FAILURE;
    }

    if (false == initializeOpenGL(window)) {
        return EXIT_FAILURE;
    }

    // create a domaine for all opengl render resource
    {
        // create shader
        const auto shaderDir = std::filesystem::path{ASSET_DIR} / "shader";
        Shader shader{shaderDir / "particles.vert", shaderDir / "particles.frag"};
        ParticleRender renderer;

        // simulation setting
        SimulationParams settings;
        settings.particleSamplingMode = ParticleSamplingMode::BySpacing;
        settings.particleSpacing = 0.08f;
        settings.kernelRadius = 2.0f * settings.particleSpacing;
        // settings.particleSamplingMode = ParticleSamplingMode::ByCount;
        // settings.particleCount = 10000;

        // initialize simulation scene
        SimulationScene scene{
            Fluids{1000.0f, InitialFluidCube{{0.0f, 1.75f, 0.0f },{1.0f, 0.75f, 1.0f}}},
            BoxContainer{{0.0f, 0.0f, 0.0f},{2.0f, 3.0f, 2.0f}}
        };

        Shader containerShader{shaderDir / "container.vert", shaderDir / "container.frag"};
        BoxContainerRender containerRender(scene.container);

        // initialize simulation manager
        SimulationManager simulationManager(scene, settings);
        bindSimulationInput(window, simulationManager); // bind input key with simulation manager

        // upload particles to GPU
        renderer.upload(simulationManager.getParticles());

        // frame the container from an oblique angle
        Camera camera{scene.container.center, -125.0f, -20.0f};
        const float containerRadius = glm::length(scene.container.halfSize);
        camera.move({0.0f, 0.0f, -2.0f * containerRadius});
        MouseState mouse;
        CameraInputSettings cameraInput;
        double lastFrameTime = glfwGetTime();

        // run loop
        while (!glfwWindowShouldClose(window)) {
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);

            if (width == 0 || height == 0) {
                glfwWaitEvents();
                continue;
            }

            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            const float aspectRatio = static_cast<float>(width) / height;

            // calculate frame delta
            const double now = glfwGetTime();
            const float frameDt = static_cast<float>(std::min(now - lastFrameTime, 0.05));  // minimun frame delta = 0.05s
            lastFrameTime = now;

            // update input for camera
            updateCamera(window, camera, mouse, frameDt, cameraInput);

            // update particles positions
            simulationManager.updateSimulation(frameDt);
            renderer.upload(simulationManager.getParticles());

            const glm::mat4 viewProjection = camera.projectionMatrix(aspectRatio) * camera.viewMatrix();

            // draw the container and particles using the same camera
            containerShader.use();
            containerShader.setMat4("viewProjection", viewProjection);
            containerRender.draw();

            shader.use();
            shader.setMat4("viewProjection", viewProjection);
            renderer.draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }   // deconstruct all opengl render resources

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}