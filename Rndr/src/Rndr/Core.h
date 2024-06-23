#pragma once

#ifdef RNDR_PLATFORM_WINDOWS
    #ifdef RNDR_BUILD_DLL
        #define RNDR_API __declspec(dllexport)
    #else
        #define RNDR_API __declspec(dllimport)
    #endif
#elif RNDR_PLATFORM_LINUX
    #ifdef RNDR_BUILD_DLL
        #define RNDR_API __attribute__((visibility("default")))
    #else
        #define RNDR_API
    #endif
#else
    #error Rndr only supports Windows or Linux!
#endif

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