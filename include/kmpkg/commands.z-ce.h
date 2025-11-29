#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandZCEMetadata;
    void command_z_ce_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
