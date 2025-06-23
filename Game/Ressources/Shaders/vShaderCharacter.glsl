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

out float bone1Influence;

void main()
{
mat4 skinMatrix = 
    inWeights.x * boneTransform[clamp(inBoneIDs.x, 0, 99)] +
    inWeights.y * boneTransform[clamp(inBoneIDs.y, 0, 99)] +
    inWeights.z * boneTransform[clamp(inBoneIDs.z, 0, 99)] +
    inWeights.w * boneTransform[clamp(inBoneIDs.w, 0, 99)];

    //skinMatrix = mat4(1.0);

    vec4 skinnedPos = skinMatrix * vec4(aPos, 1.0);
    gl_Position = projection * view * model * skinnedPos;
    FragPos = vec3(model * skinnedPos);

    vec3 skinnedNormal = mat3(skinMatrix) * aNormal;
    Normal = mat3(transpose(inverse(model))) * skinnedNormal;

	texCoords = aTexCoord;

    int boneNum = 1;

    float influence = 0.0;
    if (inBoneIDs.x == boneNum) influence += inWeights.x;
    if (inBoneIDs.y == boneNum) influence += inWeights.y;
    if (inBoneIDs.z == boneNum) influence += inWeights.z;
    if (inBoneIDs.w == boneNum) influence += inWeights.w;
    bone1Influence = influence;
}
