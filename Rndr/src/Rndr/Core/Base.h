#pragma once

#include <memory>

#ifdef RNDR_DEBUG
	#if defined(RNDR_PLATFORM_WINDOWS)
		#define RNDR_DEBUGBREAK() __debugbreak()
	#elif defined(RNDR_PLATFORM_LINUX)
		#include <signal.h>
		#define RNDR_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define RNDR_ENABLE_ASSERTS
#else
	#define RNDR_DEBUGBREAK()
#endif

//TODOL: Macro cant take single bool as argument, needs string aswell
#ifdef RNDR_ENABLE_ASSERTS
	#define RNDR_ASSERT(x, ...) { if(!(x)) { RNDR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RNDR_CORE_ASSERT(x, ...) { if(!(x)) { RNDR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RNDR_ASSERT(x, ...)
	#define RNDR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

// #define RNDR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define RNDR_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }



namespace Rndr {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}