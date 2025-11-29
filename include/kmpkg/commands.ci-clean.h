#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandCiCleanMetadata;
    void command_ci_clean_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
