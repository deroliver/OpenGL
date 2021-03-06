#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0f);
}