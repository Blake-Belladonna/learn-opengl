#version 330 core

out vec4 gl_Color;
in vec4 v2fColor;
in vec2 v2fCoord;

uniform sampler2D texture0;  
uniform sampler2D texture1;

void main()
{
    gl_Color = mix(texture(texture0, v2fCoord), texture(texture1, v2fCoord), 0.2);
}