// a post processing shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;
uniform vec2 screenSize;

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
    vec2 texel = 10.0f / textureSize(colorTarget, 0);
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

vec4 EdgeDetection(vec2 texCoord)
{
    vec2 texel = 1 / textureSize(colorTarget, 0); 
    vec4 compared = texture(colorTarget, texCoord);
    vec4 color = texture(colorTarget, texCoord + texel * vec2(1,1));

    if(distance(color.xyz, compared.xyz) > 1)
        return vec4(0,0,0,1);    

    return compared;
}

vec4 Sepia(vec2 texCoord)
{
    vec2 texel = textureSize(colorTarget, 0) * 50;
    vec4 color = texture(colorTarget, texCoord);

    float grayScale =  0.299f * color.x + 0.587f * color.y + 0.114f * color.z;
    
    color = vec4(grayScale + grayScale * .2f, grayScale, grayScale, 1);

    return color;
    
    return texture(colorTarget, texCoord);
}

vec4 Scanlines(vec2 texCoord)
{
    vec2 texel = textureSize(colorTarget, 0) * 50;
    vec4 color = texture(colorTarget, texCoord);
    
    if(texCoord.y > screenSize.y / 2)
        color *= 0.5f;

    return texture(colorTarget, texCoord);
}

vec4 Greyscale(vec2 texCoord)
{
    vec2 texel = textureSize(colorTarget, 0) * 50;
    vec4 color = texture(colorTarget, texCoord);

    float grayScale =  0.299f * color.x + 0.587f * color.y + 0.114f * color.z;
    
    color = vec4(grayScale, grayScale, grayScale, 1);

    return color;
}

vec4 Inverse(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0);
    vec4 color = texture(colorTarget, texCoord);

    color = vec4(1 - color.x, 1 - color.y, 1 - color.z, 1);

    return color;
}

vec4 Pixilizer(vec2 texCoord)
{
    vec2 texel = textureSize(colorTarget, 0) * 50;
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
    
    vec2 screenSizeCopy = screenSize;

    switch(postProcessTarget)
    {
        case 0: //Default
        FragColour = Default(texCoord); // Done
        break;
        case 1: //Distort
        FragColour = Distort(texCoord); // Done
        break;
        case 2: //Box Blur
        FragColour = BoxBlur(texCoord); // Done
        break;
        case 3: //Edge Detection
        FragColour = EdgeDetection(texCoord); // Not Done - Meh
        break;
        case 4: //Sepia
        FragColour = Sepia(texCoord); // No Idea still
        break;
        case 5: //Scanlines
        FragColour = Scanlines(texCoord); // Not Done
        break;
        case 6: //Grey scale
        FragColour = Greyscale(texCoord); // Done
        break;
        case 7: //Invert
        FragColour = Inverse(texCoord); // Done
        break;
        case 8: //Pixilizer
        FragColour = Pixilizer(texCoord); // Kinda Done
        break;
        case 9: //Posterization
        FragColour = Default(texCoord); // No Idea
        break;
        case 10: //Distance Fog
        FragColour = Default(texCoord); // Havent looked into
        break;
        case 11: //Depth of Field
        FragColour = Default(texCoord); // Havent looked into
        break;
    }

}

