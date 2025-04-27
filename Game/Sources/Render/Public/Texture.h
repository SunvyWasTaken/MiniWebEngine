#pragma once

namespace Sunset
{
	class Texture
	{
	public:
		explicit Texture(const std::string& path);
		virtual ~Texture();

		void operator()();

		void SetSubUv(const glm::vec2& uv);

		glm::vec2 GetSubUv() const;

	private:

		uint32_t texture;
		glm::vec2 m_SubUv;
	};
}
