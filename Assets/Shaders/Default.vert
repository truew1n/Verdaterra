#version 460 core

layout (location = 0) in vec3 LPosition;
layout (location = 1) in vec3 LNormal;
layout (location = 2) in vec2 LUV;

out vec3 EPosition;
out vec3 ENormal;
out vec2 EUV;

uniform mat4 UProjection;
uniform mat4 UView;
uniform mat4 UTransform;

void main()
{
    vec4 WorldPosition = UTransform * vec4(LPosition, 1.0);
    
    EPosition = WorldPosition.xyz;
    ENormal = LNormal;
    EUV = LUV;

    // gl_Position = UProjection * UView * WorldPosition;

    gl_Position = vec4(LPosition, 1.0);
}
