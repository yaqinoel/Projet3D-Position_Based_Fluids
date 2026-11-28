#include "BoxContainerRender.hpp"
#include "BoxContainer.hpp"

#include <array>
#include <cstddef>

// index of edges vertices
constexpr std::array<std::size_t, 24> edgeCorners{
    0, 1,  1, 2,  2, 3,  3, 0,  // edges on bottom
    4, 5,  5, 6,  6, 7,  7, 4,  // edges on top
    0, 4,  1, 5,  2, 6,  3, 7   // edges by side
};

using BoxVertices = std::array<float, 3 * edgeCorners.size()>;

BoxContainerRender::BoxContainerRender() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BoxVertices), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BoxContainerRender::upload(const BoxContainer& container) {
    const glm::vec3 min = container.center - container.halfSize;
    const glm::vec3 max = container.center + container.halfSize;
    const std::array<glm::vec3, 8> corners{{
        {min.x, min.y, min.z}, {max.x, min.y, min.z},
        {max.x, min.y, max.z}, {min.x, min.y, max.z},
        {min.x, max.y, min.z}, {max.x, max.y, min.z},
        {max.x, max.y, max.z}, {min.x, max.y, max.z}
    }};

    // pack the two endpoints of each edge
    BoxVertices vertices{};
    std::size_t component = 0;
    for (const std::size_t cornerIndex : edgeCorners) {
        const glm::vec3& corner = corners[cornerIndex];
        vertices[component++] = corner.x;
        vertices[component++] = corner.y;
        vertices[component++] = corner.z;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

BoxContainerRender::~BoxContainerRender() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

// draw box container in line mode
void BoxContainerRender::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(edgeCorners.size()));
    glBindVertexArray(0);
}
