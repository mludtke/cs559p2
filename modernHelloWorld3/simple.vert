#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec3 vertex_color;

uniform mat4 mvp;
flat out vec3 color;

void main()
{
	color = vertex_color;
    gl_Position = mvp * vec4(vertex_position, 1.0);
}
