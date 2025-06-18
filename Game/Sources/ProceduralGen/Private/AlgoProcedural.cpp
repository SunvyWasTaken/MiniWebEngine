// Sunset inc.

#include "AlgoProcedural.h"
#include "Mesh.h"

#include "glm/gtc/noise.hpp"

namespace Sunset
{
	void AlgoProcedural::PerlinNoise(VertexObject& data, float scale /*= 0.1f*/, float amplitude /*= 1.f*/)
	{
		for (auto& v : data.vertices)
		{
			float x = v.position.x * scale;
			float y = v.position.y * scale;
			float z = v.position.z * scale;
			v.position.y = glm::perlin(glm::vec3(x, y, z)) * amplitude;
		}
	}

	void AlgoProcedural::Erosion(VertexObject& data, int width, int height)
	{
		SCOPE_PERF
		const float talus = 0.1f;
		const float erosionFactor = 1.5f;
		const int iterations = 30;

		std::vector<float> delta(data.vertices.size(), 0.0f);

		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				int centerIndex = x + y * width;
				float centerHeight = data.vertices[centerIndex].position.y;

				for (int dy = -1; dy <= 1; ++dy)
				{
					for (int dx = -1; dx <= 1; ++dx)
					{
						if (dx == 0 && dy == 0) continue;

						int nx = x + dx;
						int ny = y + dy;
						int neighborIndex = nx + ny * width;

						float neighborHeight = data.vertices[neighborIndex].position.y;
						float diff = centerHeight - neighborHeight;

						if (diff > talus)
						{
							float transfer = (diff - talus) * erosionFactor;
							delta[centerIndex] -= transfer;
							delta[neighborIndex] += transfer;
						}
					}
				}
			}
		}

		for (size_t i = 0; i < data.vertices.size(); ++i)
		{
			data.vertices[i].position.y += delta[i];
		}
	}
}
