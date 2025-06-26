// Sunset inc.

#pragma once

namespace Sunset
{
	using ActionCallback = std::function<void(bool)>;
	using KeyCallback = std::function<void(bool, float)>;

	class InputManager final
	{
	public:
		static void KeyDispatch(const char& key, bool press);
		static void BindAction(const char& key, const ActionCallback& func);
		static void BindKey(const char& key, const KeyCallback& func);
	};
}

#define BIND_ACTION_KEY(key, Func, ptr) ::Sunset::InputManager::BindAction(key, std::bind(Func, ptr, std::placeholders::_1));
#define BIND_ACTION_KEY_LAMBDA(key, Func) ::Sunset::InputManager::BindAction(key, std::bind(Func, std::placeholders::_1));
#define BIND_TICK_KEY(key, Func, ptr) :: Sunset::InputManager::BindKey(key, std::bind(Func, ptr, std::placeholders::_1, std::placeholders::_2));
