#version 330 core

out vec4 color;

void main(){
    vec2 offset = gl_PointCoord - vec2(0.5);

    if (dot(offset, offset) > 0.25) {
        discard;
    }

    color = vec4(0.23, 0.69, 0.95, 1.0);

}