#version 460 core

out vec4 EFragmentColor;

in vec3 EPosition;
in vec3 ENormal;
in vec2 EUV;

uniform bool UHasTexture;
uniform vec4 UColorDiffuse = vec4(1.0);
uniform sampler2D UTextureDiffuse;

vec4 SampleColor()
{
    if(UHasTexture) {
        return texture(UTextureDiffuse, EUV);
    }

    return UColorDiffuse;
}

void main()
{
    EFragmentColor = SampleColor();
}