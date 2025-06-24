// Sunset inc.

#include "Animation/AnimationLoader.h"
#include "Animation/AnimationClip.h"
#include "Animation/AnimationTrack.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Sunset
{
	std::shared_ptr<AnimationClip> AnimLoader::LoadAnimation(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			ENGINE_LOG_ERROR("Loaded animation Failed {}", path)
			return nullptr;
		}

		if (scene->mNumAnimations == 0)
			return nullptr;

		aiAnimation* anim = scene->mAnimations[0];
		std::string animName = anim->mName.C_Str();
		float duration = static_cast<float>(anim->mDuration);
		float ticksPerSecond = static_cast<float>(anim->mTicksPerSecond > 0.0 ? anim->mTicksPerSecond : 25.0f);

		std::shared_ptr<AnimationClip> clip = std::make_shared<AnimationClip>(animName, duration, ticksPerSecond);

		for (unsigned int i = 0; i < anim->mNumChannels; ++i)
		{
			aiNodeAnim* channel = anim->mChannels[i];
			std::string boneName = channel->mNodeName.C_Str();
			auto track = std::make_shared<AnimationTrack>();

			// Positions
			for (unsigned int j = 0; j < channel->mNumPositionKeys; ++j)
			{
				const aiVectorKey& key = channel->mPositionKeys[j];
				KeyPosition posKey;
				posKey.time = static_cast<float>(key.mTime);
				posKey.position = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
				track->AddPositionKey(posKey);
			}

			// Rotations
			for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j)
			{
				const aiQuatKey& key = channel->mRotationKeys[j];
				KeyRotation rotKey;
				rotKey.time = static_cast<float>(key.mTime);
				rotKey.rotation = glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
				track->AddRotationKey(rotKey);
			}

			// Scales
			for (unsigned int j = 0; j < channel->mNumScalingKeys; ++j)
			{
				const aiVectorKey& key = channel->mScalingKeys[j];
				KeyScale scaleKey;
				scaleKey.time = static_cast<float>(key.mTime);
				scaleKey.scale = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
				track->AddScaleKey(scaleKey);
			}

			clip->AddTrack(boneName, track);
		}
		return clip;
	}
}
