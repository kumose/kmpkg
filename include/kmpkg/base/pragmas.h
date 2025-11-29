#pragma once

#if defined(__GNUC__) && __GNUC__ < 7
// [[nodiscard]] is not recognized before GCC version 7
#pragma GCC diagnostic ignored "-Wattributes"
#endif

#if defined(_MSC_VER)
#include <sal.h>
#endif

#if defined(_MSC_VER)
#define ASSUME(expr) __assume(expr)
#else
#define ASSUME(expr)
#endif

#define Z_KMPKG_PRAGMA(PRAGMA) _Pragma(#PRAGMA)

#if defined(_MSC_VER)
#define KMPKG_SAL_ANNOTATION(...) __VA_ARGS__
#else
#define KMPKG_SAL_ANNOTATION(...)
#endif

// the static_assert(true, "")s are to avoid the extra ';' warning
#if defined(__clang__)
// check clang first because it may define _MSC_VER
#define KMPKG_MSVC_WARNING(...)
#define KMPKG_GCC_DIAGNOSTIC(...)
#define KMPKG_CLANG_DIAGNOSTIC(DIAGNOSTIC) Z_KMPKG_PRAGMA(clang diagnostic DIAGNOSTIC)
#define KMPKG_UNUSED [[maybe_unused]]
#elif defined(_MSC_VER)
#define KMPKG_MSVC_WARNING(...) Z_KMPKG_PRAGMA(warning(__VA_ARGS__))
#define KMPKG_GCC_DIAGNOSTIC(...)
#define KMPKG_CLANG_DIAGNOSTIC(...)
#define KMPKG_UNUSED [[maybe_unused]]
#else
// gcc
#define KMPKG_MSVC_WARNING(...)
#define KMPKG_GCC_DIAGNOSTIC(DIAGNOSTIC) Z_KMPKG_PRAGMA(gcc diagnostic DIAGNOSTIC)
#define KMPKG_CLANG_DIAGNOSTIC(DIAGNOSTIC)
#define KMPKG_UNUSED __attribute__((unused))
#define KMPKG_SAL_ANNOTATION(...)
#endif
