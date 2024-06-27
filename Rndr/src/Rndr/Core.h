#pragma once

#include <memory>

#ifdef RNDR_DEBUG
	#define RNDR_ENABLE_ASSERTS
#endif

#ifdef RNDR_ENABLE_ASSERTS
	#define RNDR_ASSERT(x, ...) { if(!(x)) { RNDR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RNDR_CORE_ASSERT(x, ...) { if(!(x)) { RNDR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RNDR_ASSERT(x, ...)
	#define RNDR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RNDR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)



namespace Rndr {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}