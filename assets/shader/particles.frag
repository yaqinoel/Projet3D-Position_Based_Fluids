#version 330 core

in vec3 initialColor;   // initialColor from vertex shader
out vec4 color;

void main(){
    vec2 offset = gl_PointCoord - vec2(0.5);

    if (dot(offset, offset) > 0.10) {
        discard;
    }

    color = vec4(initialColor, 1.0);

}