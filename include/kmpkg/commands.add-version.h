#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandAddVersionMetadata;
    void command_add_version_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}