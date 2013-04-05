//influenced by the Cookbook

#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
flat in vec3 back_color;

void main()
{
    FragColor = vec4(gl_FrontFacing ? color : back_color, 1.0);
}
