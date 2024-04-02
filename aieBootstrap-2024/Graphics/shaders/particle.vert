// a Particle shader
#version 410

in vec4 Position;
in vec4 Colour;

out vec4 vColor;

uniform mat4 ProjectionViewModel;

void main()
{
    vColor = Colour;
    gl_Position = ProjectionViewModel * Position;
}