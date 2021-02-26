#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inCoord;

out vec4 v2fColor;
out vec2 v2fCoord;

uniform mat4 trans;

void main()
{
	gl_Position = trans * vec4(inPos.xyz, 1.0);
	v2fColor = vec4(inColor, 1.0);
	v2fCoord = inCoord;
}