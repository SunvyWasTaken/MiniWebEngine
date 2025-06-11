// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename Derived>
	class TextureBase
	{
	public:
		TextureBase() = default;
		virtual ~TextureBase() = default;

		template <typename ...Args>
		bool Load(Args&&... args)
		{
			Derived* derived = static_cast<Derived*>(this);
			return derived->Load(std::forward<Args>(args)...);
		}

		void Use() const
		{
			const Derived* derived = static_cast<const Derived*>(this);
			derived->Use();
		}
	protected:
		uint32_t m_Id = 0;
	};

	using Texture2D = TextureBase<class Texture>;
	using CubeMapTexture = TextureBase<class CubeTexture>;
}
