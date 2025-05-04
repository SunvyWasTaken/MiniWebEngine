#pragma once

namespace Sunset
{
	struct Inputs
	{
		struct State
		{
			struct Pressed{};
			struct Hold{};
			struct Release{};
			using Type = std::variant<Pressed, Hold>;
		};

		static bool IsKey(const int keyValue, const State::Type& type = State::Pressed{});

		static bool IsKeyReleased(const int keyValue);

		static bool IsMouseButton(const int keyValue, const State::Type& type = State::Pressed{});

		static bool IsMouseButtonReleased(const int keyValue);

		static glm::vec2 GetMouseScreenPosition();
	};
}