#include "Layer.h"

#include "imgui.h"

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
				ImGui::Text("%s", oss.str().c_str());
			},
			[&](GameDataType::Int32& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<int32_t*>(data.value));
				ImGui::Text("%s", oss.str().c_str());
			},
			[&](GameDataType::Float& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<float*>(data.value));
				ImGui::Text("%s", oss.str().c_str());
			},
			[&](GameDataType::Double& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<double*>(data.value));
				ImGui::Text("%s", oss.str().c_str());
			},
			[&](GameDataType::Vec2& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<glm::vec2*>(data.value));
				ImGui::Text("%s", oss.str().c_str());
			},
			[&](GameDataType::Vec3& tmp)
			{
				oss << data.dataName << ":" << *(static_cast<glm::vec3*>(data.value));
				ImGui::Text("%s", oss.str().c_str());
			}
			}, data.dataType);
		}
	}

	void Layer::Add(const GameData& data)
	{
		datas.emplace_back(data);
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