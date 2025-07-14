// Sunset inc.

#pragma once

namespace Sunset
{
	template<typename ...Args>
	class Callbacks
	{
		std::vector<std::function<void(Args...)>> funcs;
	public:
		void Calls(Args&&... args)
		{
			for (std::function<void(Args...)>& func : funcs)
			{
				func(std::forward<Args>(args)...);
			}
		}

		void Bind(const std::function<void(Args...)>& func)
		{
			if (func)
				funcs.emplace_back(func);
			else
				ENGINE_LOG_ERROR("The given function to the bind is not valid");
		}

		operator bool() const
		{
			return !funcs.empty();
		}
	};
}

#define CALLBACKS_MULTI(name, ...) \
class name : public ::Sunset::Callbacks<__VA_ARGS__> \
{}
