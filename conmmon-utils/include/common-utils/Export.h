// include/common-utils/Export.h
#ifndef COMMON_UTILS_EXPORT_H
#define COMMON_UTILS_EXPORT_H

#ifdef _WIN32
    // Windows 平台：MSVC, MinGW, Clang-cl 都支持 __declspec
    #ifdef common_utils_EXPORTS
        #define COMMON_UTILS_EXPORT __declspec(dllexport)
    #else
        #define COMMON_UTILS_EXPORT __declspec(dllimport)
    #endif
#else
    // Linux/macOS/其他类Unix平台
    #ifdef common_utils_EXPORTS
        #define COMMON_UTILS_EXPORT __attribute__((visibility("default")))
    #else
        #define COMMON_UTILS_EXPORT
    #endif
#endif

#endif