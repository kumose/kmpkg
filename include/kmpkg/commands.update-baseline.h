#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandUpdateBaselineMetadata;
    void command_update_baseline_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
