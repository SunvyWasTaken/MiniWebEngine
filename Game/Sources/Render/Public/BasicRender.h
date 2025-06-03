// Sunset inc.

#pragma once

namespace Sunset
{
	class Camera;

	class BasicRender
	{
	public:
		BasicRender();

		virtual ~BasicRender();

		virtual void Begin(const Camera& camera) = 0;

		virtual void End() = 0;

	protected:
		void* m_Window;
	};
}
