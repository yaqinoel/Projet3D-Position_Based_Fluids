#version 330 core

layout(location = 0) in vec3 aPosition;
uniform mat4 viewProjection;    // view and projection matrix

void main(){
    gl_Position = viewProjection * vec4(aPosition, 1.0);
    gl_PointSize = 12.0;
}