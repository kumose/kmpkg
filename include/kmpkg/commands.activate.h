#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandActivateMetadata;
    void command_activate_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
