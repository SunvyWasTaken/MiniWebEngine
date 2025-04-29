#pragma once

namespace Sunset
{
	template <class ...Ts>
	struct overload : Ts...
	{
		using Ts::operator()...;
	};

	template <class ...Ts>
	overload(Ts...) -> overload<Ts...>;
}

namespace
{
	std::ostream& operator<<(std::ostream& io, const glm::vec2& vec)
	{
		return io << "x:" << vec.x << ", y:" << vec.y;
	}

	std::ostream& operator<<(std::ostream& io, const glm::vec3& vec)
	{
		return io << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z;
	}
}

#define LOG(x) std::cerr << x << "\n";

namespace Sunset
{
	struct ScopePerfMeasure final
	{
		explicit ScopePerfMeasure(const std::string& name)
			: Name(name)
		{
			start = std::chrono::steady_clock::now();
		}
		~ScopePerfMeasure()
		{
			auto end = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			LOG(std::format("{}:{}ms", Name, duration).c_str());
		}
		std::string Name;
		std::chrono::time_point<std::chrono::steady_clock> start;
	};

	namespace Math
	{
		template <typename T>
		T lerp(const T x, const T y, const float a)
		{
			return x + (y - x) * a;
		}
	}
}

#define SCOPE_PERF(_name) Sunset::ScopePerfMeasure _scopPerfMesure_{_name};
