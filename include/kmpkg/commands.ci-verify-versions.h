#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandCiVerifyVersionsMetadata;
    void command_ci_verify_versions_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}