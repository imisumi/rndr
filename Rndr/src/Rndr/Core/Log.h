#pragma once

#include <memory>

#include "Base.h"
#include "spdlog/spdlog.h"

namespace Rndr {

	class  Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define RNDR_CORE_TRACE(...)    ::Rndr::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RNDR_CORE_INFO(...)     ::Rndr::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RNDR_CORE_WARN(...)     ::Rndr::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RNDR_CORE_ERROR(...)    ::Rndr::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RNDR_CORE_FATAL(...)    ::Rndr::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RNDR_TRACE(...)	        ::Rndr::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RNDR_INFO(...)	        ::Rndr::Log::GetClientLogger()->info(__VA_ARGS__)
#define RNDR_WARN(...)	        ::Rndr::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RNDR_ERROR(...)	        ::Rndr::Log::GetClientLogger()->error(__VA_ARGS__)
#define RNDR_FATAL(...)	        ::Rndr::Log::GetClientLogger()->fatal(__VA_ARGS__)