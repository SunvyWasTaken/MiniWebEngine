#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

#include "UniformBlock.glsl"

uniform mat4 model;

out vec3 TexCoord;

void main()
{
	gl_Position = projection * relativeView * model * vec4(aPos, 1.0);
	TexCoord = aPos * vec3(1.0, 1.0, -1.0);;
}
