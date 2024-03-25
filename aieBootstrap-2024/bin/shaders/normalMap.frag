// a normal map phong style shader
#version 410

out vec4 FragColor;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform int layerSample;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// For the light source
uniform vec3 ambientLight; // Also seen as Ia
uniform vec3 diffuseLight; // Also seen as Id
uniform vec3 specularLight; // Also seen as Is

uniform vec3 LightDirection;

// Model material colors
uniform vec3 Ka; // ambient material color
uniform vec3 Kd; // diffuse material color
uniform vec3 Ks; // specular material color
uniform float Ns; // Specular Power

uniform vec4 CameraPosition;

const int MAX_LIGHTS = 4;
uniform int NumberOfLights;
uniform vec3 PointLightColors[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);
    float specularTerm = pow(max(0,dot(R, view)), Ns); 

    return specularTerm * color;
}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(-LightDirection);

    mat3 TBN = mat3(T,B,N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = normalize(TBN * (texNormal * 2 - 1));

    // Calculate the lamber term (this is ther negaive light direction)
    float lamberTerm = max(0, min(1, dot(N, -L)));

    // Calculate the view vector 
    vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);
    
    // Calculate the reflectionvector
    vec3 R = reflect(L,N);

    vec3 specularTotal = Specular(vPosition.xyz- L, specularLight, N, V);
    vec3 diffuseTotal = Diffuse(L, diffuseLight, N);    

    for(int i = 0; i < NumberOfLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float dist = length(direction);
        direction = direction / dist;

        // Use the inverse square law to set the intensity of the lights
        vec3 color = PointLightColors[i] / (dist * dist);

        diffuseTotal += Diffuse(direction, color, N);
        specularTotal += Specular(direction, color, N, V);
    }    
    
    vec3 ambient = ambientLight * Ka * texDiffuse;
    vec3 diffuse = Kd * texDiffuse * diffuseTotal;
    vec3 specular = Ks * texSpecular * specularTotal;


    switch(layerSample)
    {
        case 0:
        FragColor = vec4(ambient + diffuse + specular, 1);
        break;

        case 1:
        FragColor = texture(diffuseTexture, vTexCoord);
        break;    

        case 2:
        FragColor = vec4(N, 1);
        break;

        case 3:
        FragColor = vec4(V, 1);
        break;

        case 4:
        FragColor = vec4(R, 1);
        break;

        case 5:
        float dist = distance(vPosition.xyz, CameraPosition.xyz)/ 100;
        FragColor = vec4(texture(diffuseTexture, vTexCoord).xyz * dist, 1);
        break;
    }
}