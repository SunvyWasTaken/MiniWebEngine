// Sunset inc.

#include "Meshes/MeshLoader.h"
#include "Meshes/SkeletalMesh.h"
#include "Meshes/StaticMesh.h"

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
	void ProcessVertexData(const aiScene* scene, vertexDataType& vertices)
	{
		aiNode* root = scene->mRootNode;

		aiMesh* mesh = scene->mMeshes[0];;

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

	void ProcessSkeletonData(const aiScene* scene, Sunset::SkeletalMeshData& data)
	{
		aiNode* root = scene->mRootNode;
		aiMesh* mesh = scene->mMeshes[0];

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
		StaticMeshData data;

		Assimp::Importer importer;

		const aiScene* scene = OpenFileAndCreateImportScene(importer, path);

		ProcessVertexData(scene, data);

		StaticMesh result{std::move(data)};

		result.m_ImportSize = GetImportScale(scene);

		return std::make_shared<Meshes>(std::move(result));
	}

	std::shared_ptr<Meshes> MeshLoader::LoadSkeletalMesh(const std::string& path) noexcept
	{
		SkeletalMeshData data;

		Assimp::Importer importer;

		const aiScene* scene = OpenFileAndCreateImportScene(importer, path);

		ProcessVertexData(scene, data);

		ProcessSkeletonData(scene, data);

		SkeletalMesh result{std::move(data)};

		result.m_ImportSize = GetImportScale(scene);

		return std::make_shared<Meshes>(std::move(result));
	}

}
