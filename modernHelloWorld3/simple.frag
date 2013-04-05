#version 400

out vec4 FragColor;

flat in vec3 color;

void main()
{
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	FragColor = vec4(color, 1.0);
}
