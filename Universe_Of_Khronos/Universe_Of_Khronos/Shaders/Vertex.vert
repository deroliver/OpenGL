#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalMatrix;

out vec3 FragPos;
out vec3 Normal;


void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0f);
	FragPos = vec3(Model * vec4(position, 1.0f));
	Normal = mat3(NormalMatrix) * normal;
}