#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandCreateMetadata;
    int command_create(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
    void command_create_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
