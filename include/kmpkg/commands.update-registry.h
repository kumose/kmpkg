#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandUpdateRegistryMetadata;
    void command_update_registry_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
