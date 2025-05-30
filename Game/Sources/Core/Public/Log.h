// Sunset inc.


namespace Sunset
{
	class Log
	{
	public:
		static void Init();
		
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
	};
}
