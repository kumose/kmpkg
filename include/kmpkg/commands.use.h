#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandUseMetadata;
    void command_use_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
