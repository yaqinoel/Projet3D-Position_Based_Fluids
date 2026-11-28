#pragma once

#include <glad/gl.h>

struct BoxContainer;

// render to draw fluid box container
class BoxContainerRender {
public:
    BoxContainerRender();
    ~BoxContainerRender();

    BoxContainerRender(const BoxContainerRender&) = delete;
    BoxContainerRender& operator=(const BoxContainerRender&) = delete;

    void upload(const BoxContainer& container);
    void draw() const;

private:
    GLuint vao = 0;
    GLuint vbo = 0;
};
