#include "Layer.h"

#include "imgui.h"

namespace
{
	Sunset::LayerContainer layerContainer;
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
			},
			[&](GameDataType::Button& tmp)
			{
				if (ImGui::Button(data.dataName.c_str()))
				{
					tmp.func();
				}
			},
			[&](GameDataType::GraphLine& tmp)
			{
				ImGui::PlotLines(data.dataName.c_str(), tmp.points->data(), tmp.points->size(), 0, data.dataName.c_str());
			}
			}, data.dataType);
		}
	}

	void Layer::Add(const GameData& data)
	{
		datas.emplace_back(data);
	}

	LayerContainer& LayerContainer::Get()
	{
		return layerContainer;
	}

	void LayerContainer::Render()
	{
		layerContainer.layers.erase(
			std::remove_if(
				layerContainer.layers.begin(),
				layerContainer.layers.end(),
				[](const std::weak_ptr<Layer>& layer) {
					return layer.expired();
				}),
			layerContainer.layers.end()
		);

		for (auto& layer : layerContainer.layers)
		{
			if (std::shared_ptr<Layer> lay = layer.lock())
			{
				ImGui::Begin(lay->LayerName.c_str());
				lay->Render();
				ImGui::End();
			}
		}
	}

	void LayerContainer::Add(const std::shared_ptr<Layer>& layer)
	{
		layers.emplace_back(layer);
	}
}