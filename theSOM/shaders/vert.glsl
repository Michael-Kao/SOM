#version 330
layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec3 COLOR;

uniform mat4 MODELVIEWPROJECTION;

out vec3 Color;

void main()
{
    gl_Position = vec4(POSITION, 1.0f);
    Color = COLOR;
}