#version 460 core

layout (location = 0) in vec3 LPosition;

uniform mat4 ULightProjection;
uniform mat4 UTransform;


void main()
{
    vec4 WorldPosition = UTransform * vec4(LPosition, 1.0);
    gl_Position = ULightProjection * WorldPosition;
}  