// Sunset inc.

#pragma once

namespace Sunset
{
	struct AnyTexture
	{
		std::function<void()> Use;
		std::shared_ptr<void> keeper;
		explicit operator bool() const noexcept
		{
			return Use.operator bool();
		}
	};

	template <typename Derived>
	class TextureBase : public std::enable_shared_from_this<TextureBase<Derived>>
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

		operator AnyTexture()
		{
			auto shared = this->shared_from_this();
			return { [this](){ Use(); }, std::static_pointer_cast<void>(shared) };
		}

	protected:
		uint32_t m_Id = 0;
	};

	using Texture2D = TextureBase<class Texture>;
	using CubeMapTexture = TextureBase<class CubeTexture>;
}
