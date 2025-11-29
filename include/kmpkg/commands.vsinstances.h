#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandVsInstancesMetadata;
    void command_vs_instances_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
