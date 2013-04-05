//influenced from the Cookbook by David

#version 400

layout (location = 0) out vec4 FragColor;

//Don't actually need back color because that face is never seen
in vec3 color;
in vec3 back_color;

void main()
{
    FragColor = vec4(gl_FrontFacing ? color : back_color, 1.0);
}
