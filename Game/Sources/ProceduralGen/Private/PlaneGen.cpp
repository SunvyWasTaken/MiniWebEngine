// Sunset inc.

#include "PlaneGen.h"
#include "VertexObject.h"

namespace Sunset
{
	void PlaneGen::Gen(Object& data, float width, float height, int resolutionX, int resolutionY)
	{
		data.Clear();

		float dx = width / (resolutionX - 1);
		float dy = height / (resolutionY - 1);

		for (int y = 0; y < resolutionY; ++y) {
			for (int x = 0; x < resolutionX; ++x) {
				float xpos = -width / 2.0f + x * dx;
				float ypos = -height / 2.0f + y * dy;

				Vertex v;
				v.position = { xpos, 0.0f, ypos };
				v.normal = { 0.0f, 1.0f, 0.0f };
				// v.uv = { (float)x / (resolutionX - 1), (float)y / (resolutionY - 1) };

				data.PushVertice(v);
			}
		}

		for (int y = 0; y < resolutionY - 1; ++y) {
			for (int x = 0; x < resolutionX - 1; ++x) {
				int i = y * resolutionX + x;

				// Triangle 1
				data.PushIndice(i);
				data.PushIndice(i + resolutionX);
				data.PushIndice(i + resolutionX + 1);

				// Triangle 2
				data.PushIndice(i);
				data.PushIndice(i + resolutionX + 1);
				data.PushIndice(i + 1);
			}
		}
	}

	void PlaneGen::ApplyWaveToTerrain(Object& data)
	{
		float amplitude = 0.2f;
		float frequency = 2.f;

		for (auto& v : data.vertices) 
		{
			float wave = amplitude * sinf(frequency * v.position.x);
			wave += amplitude * sinf(frequency * v.position.z);
			v.position.y = wave;
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}

	void PlaneGen::ProcessNormal(Object& data)
	{
		for (Vertex& v : data.vertices)
			v.normal = glm::vec3(0.0f);

		for (size_t i = 0; i < data.indices.size(); i += 3) {
			Vertex& v0 = data.vertices[data.indices[i]];
			Vertex& v1 = data.vertices[data.indices[i + 1]];
			Vertex& v2 = data.vertices[data.indices[i + 2]];

			glm::vec3 edge1 = v1.position - v0.position;
			glm::vec3 edge2 = v2.position - v0.position;

			glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

			v0.normal += faceNormal;
			v1.normal += faceNormal;
			v2.normal += faceNormal;
		}

		for (Vertex& v : data.vertices)
			v.normal = glm::normalize(v.normal);
	}
}
