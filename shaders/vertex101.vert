#version 410 core

in vec4 Position;

void main() {
    gl_Position = vec4(Position.x, Position.y, Position.z, Position.w);
}