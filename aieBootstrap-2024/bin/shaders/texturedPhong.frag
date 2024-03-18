// a simple textured shader
#version 410

in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

out vec4 FragColor;
in vec4 vPosition;
in vec3 vNormal;

uniform vec3 ambientLight; // Also seen as Ia
uniform vec3 diffuseLight; // Also seen as Id
uniform vec3 specularLight; // Also seen as Is

uniform vec3 LightDirection;

// Model material colors
uniform vec3 Ka; // ambient material color
uniform vec3 Kd; // diffuse material color
uniform vec3 Ks; // specular materil color
uniform float Ns; // Specular Power

uniform vec4 CameraPosition;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Calculate the lamber term (this is ther negaive light direction)
    float lamberTerm = max(0, min(1, dot(N, -L)));

    // Calculate the view vector 
    vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);
    
    // Calculate the reflectionvector
    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0,dot(R, V)), Ns); 

    vec3 ambient = ambientLight * Ka;
    vec3 diffuse = diffuseLight * Kd * lamberTerm;
    vec3 specular = specularLight * Ks * specularTerm;

    //(ambient + diffuse + specular);
    // texture(diffuseTexture, vTexCoord) +
    FragColor =  texture(diffuseTexture, vTexCoord) + (ambient + diffuse + specular, 1);
}