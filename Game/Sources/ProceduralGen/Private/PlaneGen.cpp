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
}
