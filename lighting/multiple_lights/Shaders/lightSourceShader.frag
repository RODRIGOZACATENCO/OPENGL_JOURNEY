#version 330 core
out vec4 FragColor;
uniform vec3 bulbColor;

void main()
{

FragColor = vec4(bulbColor,1.0f);
}