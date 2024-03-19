// a post processing shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;

out vec4 FragColor;

vec4 Default(vec2 texCoord)
{
    return texture(colorTarget, texCoord);
}

vec4 Distort(vec2 texCood)
{
    vec2 mid = vec2(0.5f);
    float distanceFromCentre = distance(texCood, mid);
    vec2 normalilzedCoord = normalize(texCood - mid);
    float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalilzedCoord;

    return texture(colorTarget, newCoord);
}

void main()
{
    // First we want to calculate the texel's size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Set the scale
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f; 

    if(postProcessTarget == 0)
        FragColor = Default(texCoord);
    else
        FragColor = Distort(texCoord);

}