// a simple textured shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoord;

uniform mat4 ProjectionViewModel;
out vec2 vTexCoord;

out vec4 vPosition;
out vec3 vNormal;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vTexCoord = TexCoord;
    vPosition = ModelMatrix * Position;
    vNormal = NormalMatrix * Normal.xyz;
    gl_Position = ProjectionViewModel * Position;
}