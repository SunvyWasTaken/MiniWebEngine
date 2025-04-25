#include "Layer.h"

#include "imgui.h"

#include "glm/glm.hpp"

#include <sstream>

namespace
{
	template <class ...Ts>
	struct overload : Ts...
	{
		using Ts::operator()...;
	};

	template <class ...Ts>
	overload(Ts...) -> overload<Ts...>;

	std::ostream& operator<<(std::ostream& io, const glm::vec2& vec)
	{
		return io << "x:" << vec.x << ", y:" << vec.y;
	}

	std::ostream& operator<<(std::ostream& io, const glm::vec3& vec)
	{
		return io << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z;
	}
}

namespace Sunset
{
	Layer::Layer(const std::string& name)
		: LayerName(name)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::Render()
	{
		for (auto& data : datas)
		{
			std::ostringstream oss;
			std::visit(overload{
			[&](GameDataType::Bool& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<bool*>(data.value));
			},
			[&](GameDataType::Int32& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<int32_t*>(data.value));
			},
			[&](GameDataType::Float& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<float*>(data.value));
			},
			[&](GameDataType::Double& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<double*>(data.value));
			},
			[&](GameDataType::Vec2& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<glm::vec2*>(data.value));
			},
			[&](GameDataType::Vec3& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<glm::vec3*>(data.value));
			}
				}, data.dataType);
			ImGui::Text("%s", oss.str().c_str());
		}
	}

	void LayerContainer::Render()
	{
		for (auto& layer : layers)
		{
			ImGui::Begin(layer->LayerName.c_str());
			layer->Render();
			ImGui::End();
		}
	}

	void LayerContainer::operator()(const std::shared_ptr<Layer>& layer)
	{
		layers.emplace_back(layer);
	}
}