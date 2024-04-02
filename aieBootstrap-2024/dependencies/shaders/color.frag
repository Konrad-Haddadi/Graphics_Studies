// a simple colorable shader
#version 410

out vec4 FragColor;

uniform vec4 BaseColour;

void main()
{
    FragColor = BaseColour;
}