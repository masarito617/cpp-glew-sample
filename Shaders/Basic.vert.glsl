#version 330

in vec3 inPosition;

void main() {
    // w成分を加える
    gl_Position = vec4(inPosition, 1.0);
}
