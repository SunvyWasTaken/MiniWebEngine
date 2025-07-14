// Sunset inc.

#include "ComponentsUpdate.h"

namespace
{
	struct ComponentsUpdate
	{
		uint32_t id;
		std::function<void(float)> func;
	};
	std::vector<ComponentsUpdate> funcs;
}

namespace Sunset
{
	void ComponentUpdater::Update(float deltatime)
	{
		for (const auto& [id, func] : funcs)
		{
			if (func)
			{
				func(deltatime);
			}
		}
	}

	void ComponentUpdater::AddComponent(uint32_t id, const std::function<void(float)>& func)
	{
		funcs.emplace_back(id, func);
	}

	void ComponentUpdater::RemoveComp(uint32_t id)
	{
		auto it = std::remove_if(funcs.begin(), funcs.end(), [id](const ComponentsUpdate& f)
			{
				return f.id == id;
			});

		if (it != funcs.end())
		{
			funcs.erase(it);
		}
	}

	void ComponentUpdater::Clear()
	{
		funcs.clear();
	}
}
