#version 460 core

out vec4 EFragmentColor;

in vec3 EPosition;
in vec3 ENormal;
in vec2 EUV;

uniform sampler2D UDiffuse;

void main()
{
    EFragmentColor = texture(UDiffuse, EUV);
}