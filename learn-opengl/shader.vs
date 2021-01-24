#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 inColor;

out vec4 outColor;
uniform float offset;

void main()
{
    gl_Position = vec4(pos.x + offset, pos.y, pos.z, 1.0);
    outColor = vec4(gl_Position);
}