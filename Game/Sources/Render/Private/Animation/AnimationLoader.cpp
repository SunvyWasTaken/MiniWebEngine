// Sunset inc.

#include "Animation/AnimationClip.h"
#include "Animation/AnimationLoader.h"
#include "Animation/AnimationTrack.h"
#include "Meshes/Bone.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace
{
	const aiScene* GetAssimpScene(Assimp::Importer& importer, const std::string& path)
	{
		const aiScene* scene = importer.ReadFile(path.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			ENGINE_LOG_ERROR("Loaded animation Failed {}", path)
				return nullptr;
		}
		return scene;
	}

	std::shared_ptr<Sunset::AnimationClip> CreateAnimationClip(const aiAnimation* anim)
	{
		std::string animName = anim->mName.C_Str();
		float duration = static_cast<float>(anim->mDuration);
		float ticksPerSecond = static_cast<float>(anim->mTicksPerSecond > 0.0 ? anim->mTicksPerSecond : 25.0f);
		return std::make_shared<Sunset::AnimationClip>(animName, duration, ticksPerSecond);
	}

	void GetUnorderedTrack(const aiAnimation* anim, std::unordered_map<std::string, std::shared_ptr<Sunset::AnimationTrack>>& trackMap)
	{
		for (unsigned int i = 0; i < anim->mNumChannels; ++i)
		{
			aiNodeAnim* channel = anim->mChannels[i];
			std::string boneName = channel->mNodeName.C_Str();
			auto track = std::make_shared<Sunset::AnimationTrack>();

			// Positions
			for (unsigned int j = 0; j < channel->mNumPositionKeys; ++j)
			{
				const aiVectorKey& key = channel->mPositionKeys[j];
				Sunset::KeyPosition posKey;
				posKey.time = static_cast<float>(key.mTime);
				posKey.position = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
				track->AddPositionKey(posKey);
			}

			// Rotations
			for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j)
			{
				const aiQuatKey& key = channel->mRotationKeys[j];
				Sunset::KeyRotation rotKey;
				rotKey.time = static_cast<float>(key.mTime);
				rotKey.rotation = glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
				track->AddRotationKey(rotKey);
			}

			// Scales
			for (unsigned int j = 0; j < channel->mNumScalingKeys; ++j)
			{
				const aiVectorKey& key = channel->mScalingKeys[j];
				Sunset::KeyScale scaleKey;
				scaleKey.time = static_cast<float>(key.mTime);
				scaleKey.scale = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
				track->AddScaleKey(scaleKey);
			}
			trackMap[boneName] = track;
		}
	}
}

namespace Sunset
{
	std::shared_ptr<AnimationClip> AnimLoader::LoadAnimation(const std::string& path, const std::vector<Bone>& boneNames)
	{
		Assimp::Importer importer;

		const aiScene* scene = GetAssimpScene(importer, path);
		assert(scene);

		if (scene->mNumAnimations == 0)
			return nullptr;

		aiAnimation* anim = scene->mAnimations[0];

		std::shared_ptr<AnimationClip> clip = CreateAnimationClip(anim);

		std::unordered_map<std::string, std::shared_ptr<Sunset::AnimationTrack>> trackMap;

		GetUnorderedTrack(anim, trackMap);

		clip->m_Tracks.reserve(boneNames.size());
		for (const auto& boneName : boneNames)
		{
			auto it = trackMap.find(boneName.name);
			if (it != trackMap.end())
				clip->m_Tracks.push_back(it->second);
			else
				clip->m_Tracks.push_back(nullptr);
		}

		return clip;
	}
}
