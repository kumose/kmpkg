#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/stringview.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

#define STRINGIFY(...) #__VA_ARGS__
#define MACRO_TO_STRING(X) STRINGIFY(X)

#if !defined(KMPKG_VERSION)
#error KMPKG_VERSION must be defined
#endif

#define KMPKG_VERSION_AS_STRING MACRO_TO_STRING(KMPKG_VERSION)

#if !defined(KMPKG_BASE_VERSION)
#error KMPKG_BASE_VERSION must be defined
#endif

#define KMPKG_BASE_VERSION_AS_STRING MACRO_TO_STRING(KMPKG_BASE_VERSION)

namespace kmpkg
{
    extern const StringLiteral kmpkg_executable_version;
    extern const CommandMetadata CommandVersionMetadata;
    void command_version_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
