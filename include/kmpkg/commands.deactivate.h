#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandDeactivateMetadata;
    void command_deactivate_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
