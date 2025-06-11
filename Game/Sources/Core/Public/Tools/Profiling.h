// Sunset inc.

#pragma once

namespace Sunset
{
	class ScopeMesure final
	{
	public:
		ScopeMesure(const std::string& name);
		~ScopeMesure();
	private:
		std::string m_Name;
		std::chrono::steady_clock::time_point m_now;
	};
}

#define SCOPE_PERF Sunset::ScopeMesure _scopeMesure(__FUNCTION__);
