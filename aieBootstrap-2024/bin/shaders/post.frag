// a post processing shader
#version 410

in vec2 vTexCoord;
in vec4 gl_FragCoord;

out vec4 FragColour;

uniform sampler2D colorTarget;
uniform sampler2D depthTarget;

uniform int postProcessTarget;
uniform float screenPos;
uniform float difference;
uniform float colorDifference;
uniform float whiteColor;
uniform int pixels;

float colorDif;
float scanlineTimer;
float edgeDif;
float whiteColorDif;
int pixelAmount;

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
    vec2 texel = 1.0f / textureSize(colorTarget, 0); 
    vec4 color = texture(colorTarget, texCoord);
    int count = 0;

    float dif = edgeDif;

    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(-1,1)).xyz)) > dif)
    {
        count++;
    }

    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(-1,0)).xyz)) > dif)
    {
        count++;
    }

    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(-1,-1)).xyz)) > dif)
    {
        count++;
    }

    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(0,0)).xyz)) > dif)
    {
        count++;
    }
    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(0,-1)).xyz)) > dif)
    {
        count++;
    }
    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(1,1)).xyz)) > dif)
    {
        count++;
    }
    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(1,0)).xyz)) > dif)
    {
        count++;
    }

    if(distance(normalize(color.xyz), normalize(texture(colorTarget, texCoord + texel * vec2(1,-1)).xyz)) > dif)
    {
        count++;
    } 


    if(count > 0)
    {
        return vec4(0,0,0,1);
    }
    else
    {
        return color;
    }
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

    if(sin((texCoord.y + scanlineTimer) * 1000) > 0)
        color = vec4(color.x * 0.75f, color.y * 0.75f, color.z * 0.75f, 1);

    return color;
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
    vec2 texel = textureSize(colorTarget, 0) * pixelAmount;
    vec4 color = texture(colorTarget, texCoord + texel);

    return color;
}


vec4 Posterization(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0); 
    vec4 color = texture(colorTarget, texCoord);

    float dif = colorDif;

    color = vec4(round(color.xyz * dif) / dif, 1);

    return color;
}

vec4 Distance(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0); 
    vec4 color = texture(colorTarget, texCoord);
    vec4 colorDepth = texture(depthTarget, texCoord);

    return vec4(color.xyz, colorDepth.r);
}

vec4 WhiteDetection(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0); 
    vec4 color = texture(colorTarget, texCoord);

    vec4 edgeColor =  EdgeDetection(texCoord);

    if(edgeColor.x == 0 && edgeColor.y == 0 && edgeColor.y == 0)
        return edgeColor;

    if(abs(color.x - color.y - color.z) > whiteColorDif)
        return vec4(1,1,1,1);

    return color;
}

void main()
{
    // First we want to calculate the texel's size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Set the scale
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f; 
    
    scanlineTimer = screenPos;
    edgeDif = difference / 10;
    pixelAmount = pixels * 10;
    colorDif = colorDifference;
    whiteColorDif = whiteColor;

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
        FragColour = EdgeDetection(texCoord); // Done
        break;

        case 4: //Sepia
        FragColour = Sepia(texCoord); // Done
        break;

        case 5: //Scanlines
        FragColour = Scanlines(texCoord); // Done
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
        FragColour = Posterization(texCoord); // No Idea
        break;

        case 10: //Distance Fog
        FragColour = Distance(texCoord); // Havent looked into
        break;

        case 11: //Depth of Field
        FragColour = Default(texCoord); // Havent looked into
        break;

        case 12: //White EdgeDetection
        FragColour = WhiteDetection(texCoord); // Havent looked into
        break;
    }

}

