// a simple shader
#version 410

out vec4 FragColor;

in vec3 vNormal;

uniform vec3 LightDirection;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Calculate the lamber term (this is ther negaive light direction)
    float lamberTerm = max(0, min(1, dot(N, -L)));
    FragColor = vec4(lamberTerm, lamberTerm,lamberTerm, 1);
}