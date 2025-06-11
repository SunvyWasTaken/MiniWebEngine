// Sunset inc.

#include "Tools/Profiling.h"

namespace Sunset
{
	ScopeMesure::ScopeMesure(const std::string& name)
		: m_Name(name)
	{
		m_now = std::chrono::steady_clock::now();
	}

	ScopeMesure::~ScopeMesure()
	{
		std::chrono::duration<float> delta = std::chrono::steady_clock::now() - m_now;
		ENGINE_LOG_TRACE("{} : {}s", m_Name.c_str(), delta.count())
	}
}
