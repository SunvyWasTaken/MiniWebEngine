// Sunset inc.

#pragma once

namespace Sunset
{
	class ComponentUpdater
	{
	public:
		static void Update(float deltatime);
		static void AddComponent(uint32_t id, const std::function<void(float)>& func);
		static void RemoveComp(uint32_t id);
		static void Clear();
	};
}
