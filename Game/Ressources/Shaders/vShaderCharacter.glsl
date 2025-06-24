#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 inBoneIDs;
layout (location = 4) in vec4 inWeights;

#include "UniformBlock.glsl"

layout (std140, binding = 1) uniform Bones
{
	mat4 boneTransform[100];
};

uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoords;

void main()
{
mat4 skinMatrix = 
    inWeights.x * boneTransform[inBoneIDs.x]+
    inWeights.y * boneTransform[inBoneIDs.y]+
    inWeights.z * boneTransform[inBoneIDs.z]+
    inWeights.w * boneTransform[inBoneIDs.w];

    //skinMatrix = mat4(1.0);

    vec4 skinnedPos = skinMatrix * vec4(aPos, 1.0);
    gl_Position = projection * view * model * skinnedPos;
    FragPos = vec3(model * skinnedPos);

    vec3 skinnedNormal = mat3(skinMatrix) * aNormal;
    Normal = mat3(transpose(inverse(model))) * skinnedNormal;

	texCoords = aTexCoord;
}
