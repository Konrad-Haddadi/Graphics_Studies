// a post processing shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;

out vec4 FragColour;

vec4 Default(vec2 texCoord)
{
    return texture(colorTarget, texCoord);
}

vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromCentre = distance(texCoord, mid);
    vec2 normalilzedCoord = normalize(texCoord - mid);
    float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalilzedCoord;

    return texture(colorTarget, newCoord);
}

vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0);
    vec4 color = texture(colorTarget, texCoord);

    color += texture(colorTarget, texCoord + texel * vec2(-1,1));
    color += texture(colorTarget, texCoord + texel * vec2(-1,0));
    color += texture(colorTarget, texCoord + texel * vec2(-1,-1));

    color += texture(colorTarget, texCoord + texel * vec2(0,0));
    color += texture(colorTarget, texCoord + texel * vec2(0,-1));

    color += texture(colorTarget, texCoord + texel * vec2(1,1));
    color += texture(colorTarget, texCoord + texel * vec2(1,0));
    color += texture(colorTarget, texCoord + texel * vec2(1,-1));

    return color / 9;
}

void main()
{
    // First we want to calculate the texel's size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Set the scale
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f; 

    switch(postProcessTarget)
    {
        case 0: //Default
        FragColour = Default(texCoord);
        break;
        case 1: //Distort
        FragColour = Distort(texCoord);
        break;
        case 2: //Box Blur
        FragColour = BoxBlur(texCoord);
        break;
        case 3: //Edge Detection
        FragColour = Default(texCoord);
        break;
        case 4: //Sepia
        FragColour = Default(texCoord);
        break;
        case 5: //Scanlines
        FragColour = Default(texCoord);
        break;
        case 6: //Grey scale
        FragColour = Default(texCoord);
        break;
        case 7: //Invert
        FragColour = Default(texCoord);
        break;
        case 8: //Pixilizer
        FragColour = Default(texCoord);
        break;
        case 9: //Posterization
        FragColour = Default(texCoord);
        break;
        case 10: //Distance Fog
        FragColour = Default(texCoord);
        break;
        case 11: //Depth of Field
        FragColour = Default(texCoord);
        break;
    }

}

