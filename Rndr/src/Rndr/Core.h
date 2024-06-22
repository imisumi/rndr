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
