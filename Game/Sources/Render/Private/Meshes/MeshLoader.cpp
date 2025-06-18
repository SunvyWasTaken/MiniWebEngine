// Sunset inc.

#include "Meshes/MeshLoader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
	void ProcessMesh(aiMesh* mesh, Sunset::VertexObject& vertexObject)
	{
		vertexObject.Clear();

		vertexObject.ReserveVertices(mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
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

			vertexObject.PushVertice(vertex);
		}

		vertexObject.ReserveIndices(mesh->mNumFaces);

		for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				uint32_t index = face.mIndices[j];
				vertexObject.PushIndice(index);
			}
		}
	}

	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Sunset::VertexObject>& vertexObjects)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Sunset::VertexObject vo;
			ProcessMesh(mesh, vo);
			vertexObjects.emplace_back(vo);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, vertexObjects);
		}
	}
}

namespace Sunset
{
	Sunset::VertexObject MeshLoader::LoadMesh(const std::string& path)
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

		std::vector<Sunset::VertexObject> vertexObjects;
		ProcessNode(scene->mRootNode, scene, vertexObjects);
	
		return vertexObjects[0];
	}
}
