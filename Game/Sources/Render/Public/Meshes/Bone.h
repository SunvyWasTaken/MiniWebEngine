// Sunset inc.

#pragma once

namespace Sunset
{
	struct Bone
	{
		std::string name;
		int parentIndex;
		glm::mat4 offsetMatrix;
		glm::mat4 localTransform;
	};
}
