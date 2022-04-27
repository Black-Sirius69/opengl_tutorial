#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 our_color;
out vec3 color;

void main() {
    gl_Position = position;
    color = our_color;
}

#shader fragment
#version 330 core

out vec4 color;
in vec3 our_color;

void main() {
    color = vec4(our_color, 1.0);
}