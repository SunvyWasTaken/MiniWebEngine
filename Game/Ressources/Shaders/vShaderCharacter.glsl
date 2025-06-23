#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 inBoneIDs;
layout (location = 4) in vec4 inWeights;

#include "UniformBlock.glsl"

uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	texCoords = aTexCoord;
}
