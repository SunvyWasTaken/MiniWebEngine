// Sunset inc.

#include "Meshes/MeshLoader.h"
#include "Meshes/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
	void ProcessMesh(aiMesh* mesh, Sunset::VerticeType& vertexObject)
	{

		if (mesh->HasBones())
			vertexObject = Sunset::SkeletalVertices();

		else
			vertexObject = Sunset::StaticVertices();

		std::visit(overloads
		{
			[&](Sunset::SkeletalVertices& vertices)
			{

			},
			[&](Sunset::StaticVertices& vertices)
			{
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


					Sunset::Vertex vertex{position, normal, texCoord};

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
		}, vertexObject);
	}

	void ProcessNode(aiNode* node, const aiScene* scene, Sunset::VerticeType& vertexObjects)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
		ProcessMesh(mesh, vertexObjects);
	}
}

namespace Sunset
{
	void MeshLoader::LoadMesh(const std::string& path, VerticeType& vertices, float& m_ImportScale)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ENGINE_LOG_FATAL("Erreur Assimp: {}", importer.GetErrorString());
		}

		ENGINE_LOG_TRACE("Modele load with success !")
		ENGINE_LOG_TRACE("Nombre de meshes : {}", scene->mNumMeshes);

		ProcessNode(scene->mRootNode, scene, vertices);

		float scaleFactor;
		if (scene->mMetaData && scene->mMetaData->Get("UnitScaleFactor", scaleFactor))
		{
			m_ImportScale = scaleFactor * 0.01f;
		}
		else
		{
			m_ImportScale = 1.0f; // fallback
		}
	}
}
