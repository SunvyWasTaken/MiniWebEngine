// Sunset inc.

#include "Inputs/InputManager.h"

namespace
{
	using ActionCallbacks = std::vector<Sunset::ActionCallback>;
	using KeyCallbacks = std::vector<Sunset::KeyCallback>;

	std::unordered_map<char, ActionCallbacks> keyAction;
	std::unordered_map<char, KeyCallbacks> keyTick;
}

namespace Sunset
{
	void InputManager::KeyDispatch(const char& key, bool press)
	{
		if (!keyAction.contains(key))
			return;

		ActionCallbacks& inputList = keyAction.at(key);
		for (ActionCallback& func : inputList)
		{
			func(press);
		}
	}

	void InputManager::BindAction(const char& key, const ActionCallback& func)
	{
		if (keyAction.contains(key))
			keyAction[key].emplace_back(func);
		else
			keyAction.emplace(key, ActionCallbacks{func});
	}

	void InputManager::BindKey(const char& key, const KeyCallback& func)
	{
		if (keyTick.contains(key))
			keyTick[key].emplace_back(func);
		else
			keyTick.emplace(key, KeyCallbacks{func});
	}

}
