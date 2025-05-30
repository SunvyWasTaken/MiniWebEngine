// Sunset inc.

#include "Log.h"

namespace Sunset
{
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("Sunset");
	}

}
