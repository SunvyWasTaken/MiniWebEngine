#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace Sunset
{
	namespace GameDataType
	{
		struct Bool {};
		struct Int32 {};
		struct Float {};
		struct Double {};
		struct Vec2 {};
		struct Vec3 {};

		using Type = std::variant<Bool, Int32, Float, Double, Vec2, Vec3>;
	};

	struct GameData
	{
		std::string dataName;
		GameDataType::Type dataType;
		void* value = nullptr;
	};

	class Layer
	{
	public:
		Layer(const std::string& name);

		virtual ~Layer();

		virtual void Render();

		std::string LayerName;

		std::vector<GameData> datas;
	};

	class LayerContainer
	{
	public:
		void Render();

		void operator()(const std::shared_ptr<Layer>& layer);

		std::vector<std::shared_ptr<Layer>> layers;
	};
}
