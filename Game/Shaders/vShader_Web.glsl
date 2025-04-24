#version 300 es
precision mediump float;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aCoord;
layout (location = 2) in vec4 aColor;

uniform mat4 viewProj;
uniform mat4 model;

out vec2 TexCoord;
out vec4 Color;

void main()
{
	gl_Position = viewProj * model * vec4(aPos, -1.0, 1.0);
	TexCoord = aCoord;
	Color = aColor;
}
