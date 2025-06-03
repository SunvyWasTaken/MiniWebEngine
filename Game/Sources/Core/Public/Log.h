// Sunset inc.

#pragma once

#include "spdlog/spdlog.h"

namespace Sunset
{
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

#define ENGINE_LOG_TRACE(...)	::Sunset::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define ENGINE_LOG_INFO(...)	::Sunset::Log::GetCoreLogger()->info(__VA_ARGS__);
#define ENGINE_LOG_WARN(...)	::Sunset::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define ENGINE_LOG_ERROR(...)	::Sunset::Log::GetCoreLogger()->error(__VA_ARGS__);

#define LOG_TRACE(...)	::Sunset::Log::GetLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...)	::Sunset::Log::GetLogger()->info(__VA_ARGS__);
#define LOG_WARN(...)	::Sunset::Log::GetLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...)	::Sunset::Log::GetLogger()->error(__VA_ARGS__);
