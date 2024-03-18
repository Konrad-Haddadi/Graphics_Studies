// a simple colorable shader
#version 410

out vec4 FragColor;

uniform vec3 Kd;

void main()
{
    FragColor = vec4(Kd, 1);
}