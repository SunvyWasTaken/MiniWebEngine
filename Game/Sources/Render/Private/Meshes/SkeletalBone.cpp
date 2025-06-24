// Sunset inc.

#include "Meshes/SkeletalBone.h"
#include "Animation/AnimationPlayer.h"

#include <glad/glad.h>

namespace Sunset
{
	Skeletal::Skeletal()
		: m_Ubo(0)
		, m_Animation(std::unique_ptr<AnimationPlayer>(new AnimationPlayer()))
	{
		glGenBuffers(1, &m_Ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 100, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		ENGINE_LOG_INFO("Ubo skeletal bone init {}", m_Ubo);
	}

	Skeletal::~Skeletal()
	{
		if (m_Ubo)
		{
			ENGINE_LOG_INFO("Ubo skeletal bone delete {}", m_Ubo);
			glDeleteBuffers(1, &m_Ubo);
		}
	}

	Skeletal::Skeletal(Skeletal&& other) noexcept
		: m_Ubo(std::exchange(other.m_Ubo, 0))
		, bones(std::move(other.bones))
		, boneNameToIndex(std::move(other.boneNameToIndex))
		, finalBoneMatrices(std::move(other.finalBoneMatrices))
		, m_Animation(std::move(other.m_Animation))
	{
	}

	Skeletal& Skeletal::operator=(Skeletal&& other) noexcept
	{
		if (this != &other)
		{
			m_Ubo = std::exchange(other.m_Ubo, 0);
			bones = std::move(other.bones);
			boneNameToIndex = std::move(other.boneNameToIndex);
			finalBoneMatrices = std::move(other.finalBoneMatrices);
			m_Animation = std::move(other.m_Animation);
		}
		return *this;
	}


	void Skeletal::Update(float deltatime)
	{
		if (finalBoneMatrices.size() != bones.size())
			finalBoneMatrices.resize(bones.size(), glm::mat4(1.0f));

		// Calcul des globalTransforms
		std::vector<glm::mat4> globalTransforms(bones.size(), glm::mat4(1.0f));

		if (m_Animation->IsPlaying())
		{
			std::vector<glm::mat4> localTransforms;

			m_Animation->Update(deltatime);
			m_Animation->GetPose(localTransforms);

			for (size_t i = 0; i < bones.size(); ++i)
			{
				const Bone& bone = bones[i];
				if (bone.parentIndex >= 0)
					globalTransforms[i] = globalTransforms[bone.parentIndex] * localTransforms[i];
				else
					globalTransforms[i] = localTransforms[i];

				finalBoneMatrices[i] = globalTransforms[i] * bone.offsetMatrix;
			}

			return;
		}

		for (size_t i = 0; i < bones.size(); ++i)
		{
			const Bone& bone = bones[i];
			if (bone.parentIndex >= 0)
				globalTransforms[i] = globalTransforms[bone.parentIndex] * bone.localTransform;
			else
				globalTransforms[i] = bone.localTransform;

			finalBoneMatrices[i] = globalTransforms[i] * bone.offsetMatrix;
		}
	}

	void Skeletal::UpdateGPU() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_Ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * finalBoneMatrices.size(), finalBoneMatrices.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Skeletal::operator()(std::shared_ptr<AnimationClip>& anim)
	{
		m_Animation->Play(anim);
	}

}
