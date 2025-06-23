// Sunset inc.

#include "Meshes/MeshLoader.h"
#include "Meshes/SkeletalMesh.h"
#include "Meshes/StaticMesh.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace
{
	const aiScene* OpenFileAndCreateImportScene(Assimp::Importer& importer, const std::string& path)
	{
		const aiScene* scene = importer.ReadFile(path.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ENGINE_LOG_FATAL("Erreur Assimp: {}", importer.GetErrorString());
			return nullptr;
		}

		ENGINE_LOG_TRACE("Modele load with success !");
		ENGINE_LOG_TRACE("Nombre de meshes : {}", scene->mNumMeshes);
		ENGINE_LOG_TRACE("Nombre de meshes in the root : {}", scene->mRootNode->mNumMeshes);

		return scene;
	}

	template <typename vertexDataType>
	void ProcessVertexData(aiMesh* mesh, vertexDataType& vertices)
	{
		assert(mesh);

		vertices.Clear();
		vertices.ReserveVertices(mesh->mNumVertices);

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;

			glm::vec3 normal(0.0f);
			if (mesh->HasNormals())
			{
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
			}

			glm::vec2 texCoord(0.0f);
			if (mesh->mTextureCoords[0])
			{
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;
			}

			typename vertexDataType::Type vertex{ position, normal, texCoord };

			vertices.PushVertice(vertex);
		}

		vertices.ReserveIndices(mesh->mNumFaces);

		for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				uint32_t index = face.mIndices[j];
				vertices.PushIndice(index);
			}
		}
	}

	void ProcessSkeletonData(const aiMesh* mesh, Sunset::SkeletalMeshData& data)
	{
		assert(mesh);

		if (!mesh->HasBones())
		{
			ENGINE_LOG_WARN("U load a skeletal mesh even thought there is no bones in the given fbx.")
			return;
		}

		for (unsigned int boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
		{
			aiBone* bone = mesh->mBones[boneIdx];

			for (unsigned int w = 0; w < bone->mNumWeights; ++w)
			{
				unsigned int vertexId = bone->mWeights[w].mVertexId;
				float weight = bone->mWeights[w].mWeight;

				for (int i = 0; i < 4; ++i)
				{
					if (data.vertices[vertexId].weights[i] == 0.0f)
					{
						data.vertices[vertexId].boneIDs[i] = boneIdx;
						data.vertices[vertexId].weights[i] = weight;
						break;
					}
				}
			}
		}

		for (auto& vertex : data.vertices)
		{
			float sum = vertex.weights[0] + vertex.weights[1] + vertex.weights[2] + vertex.weights[3];
			if (sum > 0.0f)
			{
				vertex.weights[0] /= sum;
				vertex.weights[1] /= sum;
				vertex.weights[2] /= sum;
				vertex.weights[3] /= sum;
			}
		}
	}

	glm::mat4 ConvertMatrix(const aiMatrix4x4& m)
	{
		return glm::transpose(glm::make_mat4(&m.a1));
	}

	std::string Mat4ToString(const glm::mat4& m) {
		std::ostringstream oss;
		oss << "[";
		for (int row = 0; row < 4; ++row) {
			oss << "[";
			for (int col = 0; col < 4; ++col) {
				oss << m[col][row];
				if (col < 3) oss << ", ";
			}
			oss << "]";
			if (row < 3) oss << ", ";
		}
		oss << "]";
		return oss.str();
	}

	// Recursively builds the bone hierarchy for the skeleton
	void BuildSkeletalRecursive(
		const aiNode* node,
		const aiScene* scene,
		Sunset::Skeletal& skeletal,
		int parentIndex = -1)
	{
		// Get the name of the current node
		std::string nodeName = node->mName.C_Str();

		// For each mesh in the scene, check if this node corresponds to a bone used in any mesh
		for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
		{
			aiMesh* mesh = scene->mMeshes[meshIdx];
			for (unsigned int boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
			{
				aiBone* aiBone = mesh->mBones[boneIdx];
				std::string boneName = aiBone->mName.C_Str();

				// If the node name matches a bone name
				if (boneName == nodeName)
				{
					// If the bone is not already in the skeleton, add it
					if (skeletal.boneNameToIndex.find(boneName) == skeletal.boneNameToIndex.end())
					{
						Sunset::Bone bone;
						bone.name = boneName;
						bone.offsetMatrix = ConvertMatrix(aiBone->mOffsetMatrix); // Bone offset (bind pose)
						bone.localTransform = ConvertMatrix(node->mTransformation); // Local transform from the node
						bone.parentIndex = parentIndex; // Index of the parent bone
						ENGINE_LOG_INFO("{} : offset : {}, local : {}", boneName, Mat4ToString(bone.offsetMatrix), Mat4ToString(bone.localTransform));

						int index = static_cast<int>(skeletal.bones.size());
						skeletal.bones.emplace_back(bone);
						skeletal.boneNameToIndex[boneName] = index;

						parentIndex = index; // Update parent index for children
					}
					else
					{
						// If already present, update parent index for children
						parentIndex = skeletal.boneNameToIndex[boneName];
					}
					break; // Bone found, no need to check other bones in this mesh
				}
			}
		}

		// Recursively process all child nodes
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			BuildSkeletalRecursive(node->mChildren[i], scene, skeletal, parentIndex);
		}
	}


	void BuildSkeletal(const aiScene* scene, Sunset::Skeletal& skeletal)
	{
		skeletal.bones.clear();
		skeletal.boneNameToIndex.clear();
		BuildSkeletalRecursive(scene->mRootNode, scene, skeletal, -1);
	}


	float GetImportScale(const aiScene* scene)
	{
		float scaleFactor = 1.0f;
		if (scene->mMetaData && scene->mMetaData->Get("UnitScaleFactor", scaleFactor))
			return scaleFactor;
		return 1.0f;
	}
}

namespace Sunset
{
	std::shared_ptr<Meshes> MeshLoader::LoadStaticMesh(const std::string& path) noexcept
	{
		StaticMesh result;

		Assimp::Importer importer;

		const aiScene* scene = OpenFileAndCreateImportScene(importer, path);
		
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			StaticMeshData data;

			aiMesh* mesh = scene->mMeshes[i];

			ProcessVertexData(mesh, data);

			result.AddSubMesh(data);
		}

		result.m_ImportSize = GetImportScale(scene);

		return std::make_shared<Meshes>(std::move(result));
	}

	std::shared_ptr<Meshes> MeshLoader::LoadSkeletalMesh(const std::string& path) noexcept
	{
		SkeletalMesh result;

		Assimp::Importer importer;

		const aiScene* scene = OpenFileAndCreateImportScene(importer, path);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			SkeletalMeshData data;

			aiMesh* mesh = scene->mMeshes[i];

			ProcessVertexData(mesh, data);

			ProcessSkeletonData(mesh, data);

			result.AddSubMesh(data);
		}

		BuildSkeletal(scene, result.m_Skeletal);

		result.m_ImportSize = GetImportScale(scene);

		return std::make_shared<Meshes>(std::exchange(result, {}));
	}

}
