#pragma once

namespace Sunset
{
	namespace GameDataType
	{
		struct Bool
		{
			bool value;
		};
		struct Int32 {};
		struct Float {};
		struct Double {};
		struct Vec2 {};
		struct Vec3 {};
		struct Button
		{
			std::function<void()> func;
		};

		struct GraphLine
		{
			std::vector<float>* points;
		};

		using Type = std::variant<Bool, Int32, Float, Double, Vec2, Vec3, Button, GraphLine>;
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
		explicit Layer(const std::string& name);

		virtual ~Layer();

		virtual void Render();

		void Add(const GameData& data);

	private:

		friend class LayerContainer;

		std::string LayerName;

		std::vector<GameData> datas;
	};

	class LayerContainer
	{
	public:
		
		static LayerContainer& Get();
		
		static void Render();

		void Add(const std::shared_ptr<Layer>& layer);

		std::vector<std::weak_ptr<Layer>> layers;
	};
}
