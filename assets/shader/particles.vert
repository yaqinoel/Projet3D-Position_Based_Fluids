#version 330 core

layout(location = 0) in vec3 aPosition; // particles position
layout(location = 1) in float aHeight;  // particles initial height

uniform mat4 viewProjection;    // view and projection matrix
uniform float initialShapeHeight;

out vec3 initialColor;  // initial color output to fragement shader

vec3 lowColor = vec3(0.12, 0.30, 0.76);
vec3 highColor = vec3(0.55, 0.95, 1.0);

void main(){
    gl_Position = viewProjection * vec4(aPosition, 1.0);
    float colorFraction = clamp(aHeight / initialShapeHeight, 0.0, 1.0);
    initialColor = mix(lowColor, highColor, colorFraction);
    gl_PointSize = 12.0;
}